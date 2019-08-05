#include <irrlicht.h>
#include <irrExt/irrExt.hpp>
#include <irrExt/FileSystem.hpp>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/camera.h>
#include <assimp/color4.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/Defines.h>
#include <assimp/light.h>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/postprocess.h>

std::vector < std::string >
readModelFiles( std::string const & fileName = "../../media/test/test.txt" )
{
   std::vector < std::string > fileNames = irrExt::loadStringVector( fileName );

   std::string baseDir = irrExt::getAbsoluteFilePath( fileName );
}

inline std::ostream &
operator<< ( std::ostream & out, aiColor4D const & color )
{
   out << int32_t( 255.0f * color.r ) << "," << int32_t( 255.0f * color.g ) << "," << int32_t( 255.0f * color.b ) << "," << int32_t( 255.0f * color.a );
   return out;
}

inline std::ostream &
operator<< ( std::ostream & out, aiColor3D const & color )
{
   out << int32_t( 255.0f * color.r ) << "," << int32_t( 255.0f * color.g ) << "," << int32_t( 255.0f * color.b );
   return out;
}

inline std::ostream &
operator<< ( std::ostream & out, aiPrimitiveType const & primitiveType )
{
   int k = 0;
   if ( primitiveType & aiPrimitiveType::aiPrimitiveType_POINT )
   {
      out << "EPT_POINT";
      ++k;
   }
   if ( k > 0 ) out << "|";
   if ( primitiveType & aiPrimitiveType::aiPrimitiveType_LINE )
   {
      out << "EPT_LINE";
      ++k;
   }
   if ( k > 0 ) out << "|";
   if ( primitiveType & aiPrimitiveType::aiPrimitiveType_TRIANGLE )
   {
      out << "EPT_TRIANGLE";
      ++k;
   }
   if ( k > 0 ) out << "|";
   if ( primitiveType & aiPrimitiveType::aiPrimitiveType_POLYGON )
   {
      out << "EPT_POLYGON";
      ++k;
   }
   return out;
}

inline irr::video::E_TEXTURE_CLAMP
convert( aiTextureMapMode const & texMode )
{
   irr::video::E_TEXTURE_CLAMP out = irr::video::ETC_CLAMP;
   switch( texMode )
   {
      case aiTextureMapMode_Clamp: out = irr::video::ETC_CLAMP_TO_BORDER; break;
      case aiTextureMapMode_Wrap: out = irr::video::ETC_REPEAT; break;
      case aiTextureMapMode_Mirror: out = irr::video::ETC_MIRROR; break;
      case aiTextureMapMode_Decal: out = irr::video::ETC_REPEAT; break;
      default: break;
   }
   return out;
}

inline irr::video::SColor
convert( aiColor3D const & color )
{
   irr::video::SColor out( 0xFFFFFFFF );
   out.setRed( uint32_t( 255.0f * color.r ) );
   out.setGreen( uint32_t( 255.0f * color.g ) );
   out.setBlue( uint32_t( 255.0f * color.b ) );
   return out;
}

inline irr::video::SColor
convert( aiColor4D const & color )
{
   irr::video::SColor out( 0xFFFFFFFF );
   out.setRed( uint32_t( 255.0f * color.r ) );
   out.setGreen( uint32_t( 255.0f * color.g ) );
   out.setBlue( uint32_t( 255.0f * color.b ) );
   out.setAlpha( uint32_t( 255.0f * color.a ) );
   return out;
}

inline irr::video::ITexture*
readTexture( irr::video::IVideoDriver* driver, std::string texName, std::string mediaDir )
{
   std::cout << "[Info] " << __FUNCTION__ << " :: texName(" << texName << ")\n";

   if ( texName.empty() ) return nullptr;
   if ( mediaDir.empty() ) mediaDir = irrExt::getCurrentDirectory();

   std::string fileName = mediaDir;
   fileName += "/";
   fileName += texName;

   std::cout << "[Info] " << __FUNCTION__ << " :: fileName(" << fileName << ")\n";

   irr::video::ITexture* tex = driver->getTexture( fileName.c_str() );

   // std::cout << "[Info] " << __FUNCTION__ << " :: SpecularColor(" << specularColor << ")\n";

   return tex;
}

inline bool
readMaterial( irr::video::IVideoDriver* driver,
              irr::video::SMaterial & out,
              aiScene const * const assimpScene,
              uint32_t materialIndex,
              std::string mediaDir )
{
   if ( !assimpScene ) return false;

   aiNode* assimpRootNode = assimpScene->mRootNode;
   if ( assimpRootNode )
   {
      std::cout << "[Info] " << __FUNCTION__ << " :: RootNode(" << assimpRootNode->mName.C_Str() << ")\n";
   }
   //std::cout << "[Info] " << __FUNCTION__ << " :: \n";

   uint32_t const materialCount = assimpScene->mNumMaterials;
   if ( materialIndex >= materialCount ) return false;

   aiMaterial const * const assimpMaterial = assimpScene->mMaterials[ materialIndex ];
   if ( !assimpMaterial ) return false;

   // Material name:
   aiString materialName;
   assimpMaterial->Get( AI_MATKEY_NAME, materialName );

   // Material diffuse color:
   aiColor3D diffuseColor( 0.f, 0.f, 0.f );
   assimpMaterial->Get( AI_MATKEY_COLOR_DIFFUSE, diffuseColor );

   // Material ambient color:
   aiColor3D ambientColor( 0.f, 0.f, 0.f );
   assimpMaterial->Get( AI_MATKEY_COLOR_AMBIENT, ambientColor );

   // Material specular color:
   aiColor3D specularColor( 0.f, 0.f, 0.f );
   assimpMaterial->Get( AI_MATKEY_COLOR_SPECULAR, specularColor );

   // Material specular shininess: ( full_shininess = shininess * strength )
   float32_t shininess;
   assimpMaterial->Get( AI_MATKEY_SHININESS, shininess );

   // Material specular shininess strength:
   float32_t shininessStrength;
   assimpMaterial->Get( AI_MATKEY_SHININESS_STRENGTH, shininessStrength );

   // Material emissive color:
   aiColor3D emissiveColor( 0.f, 0.f, 0.f );
   assimpMaterial->Get( AI_MATKEY_COLOR_EMISSIVE, emissiveColor );

   // Material culling?:
   int32_t twosided = 0;
   assimpMaterial->Get( AI_MATKEY_TWOSIDED, twosided );

   // Material shadingModel:
   int32_t shadingModel = 0;
   assimpMaterial->Get( AI_MATKEY_SHADING_MODEL, shadingModel );

   // Material wireframe:
   int32_t wireframe = 0;
   assimpMaterial->Get( AI_MATKEY_ENABLE_WIREFRAME, wireframe );

   // #define AI_MATKEY_BLEND_FUNC "$mat.blend",0,0
   // #define AI_MATKEY_OPACITY "$mat.opacity",0,0
   // #define AI_MATKEY_BUMPSCALING "$mat.bumpscaling",0,0
   // #define AI_MATKEY_REFLECTIVITY "$mat.reflectivity",0,0
   // #define AI_MATKEY_REFRACTI "$mat.refracti",0,0

   out.MaterialType = irr::video::EMT_SOLID;
   out.Wireframe = wireframe;
   out.Lighting = false;
   out.FogEnable = false;
   out.UseMipMaps = false;
   out.PointCloud = false;
   out.BackfaceCulling = twosided > 0;
   out.FrontfaceCulling = false;
   out.AntiAliasing = irr::video::EAAM_OFF;
   out.EmissiveColor = convert( emissiveColor );
   out.DiffuseColor = convert( diffuseColor );
   out.AmbientColor = convert( ambientColor );
   out.SpecularColor = convert( specularColor );
   out.Shininess = shininess;

   uint32_t nTexNONE = assimpMaterial->GetTextureCount( aiTextureType_NONE );
   uint32_t nTexHEIGHT = assimpMaterial->GetTextureCount( aiTextureType_HEIGHT );
   uint32_t nTexAMBIENT = assimpMaterial->GetTextureCount( aiTextureType_AMBIENT );
   uint32_t nTexDIFFUSE = assimpMaterial->GetTextureCount( aiTextureType_DIFFUSE );
   uint32_t nTexNORMALS = assimpMaterial->GetTextureCount( aiTextureType_NORMALS );
   uint32_t nTexOPACITY = assimpMaterial->GetTextureCount( aiTextureType_OPACITY );
   uint32_t nTexUNKNOWN = assimpMaterial->GetTextureCount( aiTextureType_UNKNOWN );
   uint32_t nTexEMISSIVE = assimpMaterial->GetTextureCount( aiTextureType_EMISSIVE );
   uint32_t nTexLIGHTMAP = assimpMaterial->GetTextureCount( aiTextureType_LIGHTMAP );
   uint32_t nTexSPECULAR = assimpMaterial->GetTextureCount( aiTextureType_SPECULAR );
   uint32_t nTexSHININESS = assimpMaterial->GetTextureCount( aiTextureType_SHININESS );
   uint32_t nTexREFLECTION = assimpMaterial->GetTextureCount( aiTextureType_REFLECTION );
   uint32_t nTexDISPLACEMENT = assimpMaterial->GetTextureCount( aiTextureType_DISPLACEMENT );

   uint32_t texCount = nTexNONE + nTexHEIGHT + nTexAMBIENT + nTexDIFFUSE + nTexNORMALS + nTexOPACITY + nTexUNKNOWN + nTexEMISSIVE + nTexLIGHTMAP + nTexSPECULAR + nTexSHININESS + nTexREFLECTION + nTexDISPLACEMENT;

   std::cout << "[Info] " << __FUNCTION__ << " :: Material[" << materialIndex << " of " << materialCount << "] " << materialName.C_Str() << ":\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: DiffuseColor(" << diffuseColor << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: AmbientColor(" << ambientColor << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: SpecularColor(" << specularColor << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: Shininess(" << shininess << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: Shininess(" << shininessStrength  << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: TexCount(" << texCount << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: nTexNONE(" << nTexNONE << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: nTexHEIGHT(" << nTexHEIGHT << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: nTexAMBIENT(" << nTexAMBIENT << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: nTexDIFFUSE(" << nTexDIFFUSE << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: nTexNORMALS(" << nTexNORMALS << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: nTexOPACITY(" << nTexOPACITY << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: nTexUNKNOWN(" << nTexUNKNOWN << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: nTexEMISSIVE(" << nTexEMISSIVE << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: nTexLIGHTMAP(" << nTexLIGHTMAP << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: nTexSPECULAR(" << nTexSPECULAR << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: nTexSHININESS(" << nTexSHININESS << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: nTexREFLECTION(" << nTexREFLECTION << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: nTexDISPLACEMENT(" << nTexDISPLACEMENT << ")\n";

   // aiMaterial::GetTexture( type, index, path, mapping, uvindex, blend, op, mapmode );
   aiString texName;
   aiTextureMapping texMapping;
   uint32_t uvIndex = 0;
   ai_real blend = ai_real( 0.0 );
   aiTextureOp texOp;
   aiTextureMapMode texMode;

   uint32_t maxIndex = std::min( nTexDIFFUSE, 4u ); // Irrlicht was compiled with support for 4 tex stages
   for ( uint32_t i = 0; i < maxIndex; ++i )
   {
      aiReturn ret = assimpMaterial->GetTexture( aiTextureType_DIFFUSE, i, &texName, &texMapping, &uvIndex, &blend, &texOp, &texMode );
      if ( ret != aiReturn_SUCCESS )
      {
         std::cout << "[Warn] " << __FUNCTION__ << " :: " << aiGetErrorString() << "\n";
         continue;
      }
      std::cout << "[Info] " << __FUNCTION__ << " :: DIFFUSE_TEX[" << i << "] " << texName.C_Str() << ":\n";
      std::cout << "[Info] " << __FUNCTION__ << " :: aiTextureMapping(" << texMapping << ")\n";
      std::cout << "[Info] " << __FUNCTION__ << " :: uvIndex(" << uvIndex << ")\n";
      std::cout << "[Info] " << __FUNCTION__ << " :: blend(" << blend << ")\n";
      std::cout << "[Info] " << __FUNCTION__ << " :: aiTextureOp(" << texOp << ")\n";
      std::cout << "[Info] " << __FUNCTION__ << " :: aiTextureMapMode(" << texMode << ")\n";

      if ( uvIndex < 4 )
      {
         out.TextureLayer[ uvIndex ].LODBias = 2;
         out.TextureLayer[ uvIndex ].TextureWrapU = convert( texMode );
         out.TextureLayer[ uvIndex ].TextureWrapV = convert( texMode );
         out.TextureLayer[ uvIndex ].BilinearFilter = true;
         out.TextureLayer[ uvIndex ].TrilinearFilter = true;
         out.TextureLayer[ uvIndex ].AnisotropicFilter = 4;
         if ( driver )
            out.TextureLayer[ uvIndex ].Texture = readTexture( driver, texName.C_Str(), mediaDir );
         else {
            out.TextureLayer[ uvIndex ].Texture = nullptr;
         }
      }
   }

   // aiMaterial* mat = assimp->mMaterials[ mesh->mMaterialIndex ];
   // aiMaterial* mat = assimp->mMaterials[ mesh->mMaterialIndex ];
   // aiColor4D specColor;
   // aiGetMaterialColor( mat, AI_MATKEY_COLOR_SPECULAR/DIFFUSE/AMBIENT, &specCol);
   // aiGetMaterialFloat( mat, AI_MATKEY_SHININESS, &specCol);
   // for mes->mNumVertices
   // aiVector3D ver/norm = mes->mVertices/mNormals
   // for mesh->mNumFaces;
   // indices.pb(mesh->mFaces[f].mIndices[0,1,2]);

   // AutoMeshBuffer* outMesh = new AutoMeshBuffer();

   // outMesh->

   //out.DiffuseColor.set( )

   return true;
}

inline uint32_t
computeIndexCount( aiScene const * assimpScene, uint32_t meshIndex )
{
   if ( !assimpScene )
   {
      std::cout << "[Error] " << __FUNCTION__ << " :: Invalid pointer to aiScene\n";
      return 0;
   }

   uint32_t const meshCount = assimpScene->mNumMeshes;
   if ( meshIndex >= meshCount )
   {
      std::cout << "[Error] " << __FUNCTION__ << " :: Invalid meshIndex(" << meshIndex << ") of meshCount(" << meshCount << ")\n";
      return 0;
   }

   aiMesh const * assimpMesh = assimpScene->mMeshes[ meshIndex ];
   if ( !assimpMesh )
   {
      std::cout << "[Error] " << __FUNCTION__ << " :: Invalid pointer to aiMesh for meshIndex(" << meshIndex << ")\n";
      return 0;
   }

   uint32_t indexCount = 0;
   uint32_t faceCount = assimpMesh->mNumFaces;

   for ( uint32_t i = 0; i < faceCount; ++i )
   {
      aiFace const & face = assimpMesh->mFaces[ i ];
      indexCount += face.mNumIndices;
   }

   return indexCount;
}


inline irrExt::AutoMeshBuffer*
readMesh( irr::video::IVideoDriver* driver, aiScene const * assimpScene, uint32_t meshIndex, std::string mediaDir )
{
   // for mes->mNumVertices
   // aiVector3D ver/norm = mes->mVertices/mNormals
   // for mesh->mNumFaces;
   // indices.pb(mesh->mFaces[f].mIndices[0,1,2]);

   // AutoMeshBuffer* outMesh = new AutoMeshBuffer();

   // outMesh->
   if ( !assimpScene )
   {
      std::cout << "[Error] " << __FUNCTION__ << " :: Invalid pointer to aiScene\n";
      return nullptr;
   }

   uint32_t const meshCount = assimpScene->mNumMeshes;
   if ( meshIndex >= meshCount )
   {
      std::cout << "[Error] " << __FUNCTION__ << " :: Invalid meshIndex(" << meshIndex << ") of meshCount(" << meshCount << ")\n";
      return nullptr;
   }

   aiMesh const * assimpMesh = assimpScene->mMeshes[ meshIndex ];
   if ( !assimpMesh )
   {
      std::cout << "[Error] " << __FUNCTION__ << " :: Invalid pointer to aiMesh for meshIndex(" << meshIndex << ")\n";
      return nullptr;
   }

   irrExt::AutoMeshBuffer* outMesh = new irrExt::AutoMeshBuffer( irr::scene::EPT_TRIANGLES );

   uint32_t materialIndex = assimpMesh->mMaterialIndex;

   std::cout << "[Info] " << __FUNCTION__ << " :: meshIndex(" << meshIndex << "/" << meshCount << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: materialIndex(" << materialIndex << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: mediaDir(" << mediaDir << ")\n";

   if ( !readMaterial( driver, outMesh->Material, assimpScene, materialIndex, mediaDir ) )
   {
      std::cout << "[Error] " << __FUNCTION__ << " :: Cannot read materialIndex(" << materialIndex << ")" << std::endl;
      return nullptr;
   }

   // for mes->mNumVertices
   // aiVector3D ver/norm = mes->mVertices/mNormals
   // for mesh->mNumFaces;
   // indices.pb(mesh->mFaces[f].mIndices[0,1,2]);

   uint32_t vertexCount = assimpMesh->mNumVertices;

   outMesh->Vertices.reserve( vertexCount );

   aiVector3D a_pos;
   aiVector3D a_nrm;
   aiColor4D a_col;
   aiVector3D a_tx0;

   // ============================
   // Parse vertices:
   // ============================
   for ( uint32_t i = 0; i < vertexCount; ++i )
   {
      // Reset each iteration
      a_nrm = aiVector3D( 0.0f, 0.0f, 1.0f );
      a_col = aiColor4D( 1.0f, 1.0f, 1.0f, 1.0f );
      a_tx0 = aiVector3D( 0.0f, 0.0f, 0.0f );

      // Read vertex position
      a_pos = assimpMesh->mVertices[ i ];

      // Read vertex normal
      if ( assimpMesh->HasNormals() )
      {
         a_nrm = assimpMesh->mNormals[ i ];
      }

      // Read vertex color
      if ( assimpMesh->HasVertexColors( 0 ) )
      {
         a_col = assimpMesh->mColors[ 0 ][ i ];
      }

      // Read vertex tex-coord 0
      if ( assimpMesh->HasTextureCoords( 0 ) )
      {
         a_tx0 = assimpMesh->mTextureCoords[ 0 ][ i ];
      }

      // Write irrlicht vertex
      outMesh->Vertices.emplace_back(
                              irr::core::vector3df( a_pos.x, a_pos.z, a_pos.y ),
                              irr::core::vector3df( a_nrm.x, a_nrm.z, a_nrm.y ),
                              convert( a_col ),
                              irr::core::vector2df( a_tx0.x, a_tx0.y ) );
   }

   // for mes->mNumVertices
   // aiVector3D ver/norm = mes->mVertices/mNormals
   // for mesh->mNumFaces;
   // indices.pb(mesh->mFaces[f].mIndices[0,1,2]);


   // ============================
   // Parse faces:
   // ============================

   uint32_t faceCount = assimpMesh->mNumFaces;

   uint32_t indexCount = computeIndexCount( assimpScene, meshIndex );

   outMesh->Indices.reserve( indexCount );

   for ( uint32_t i = 0; i < faceCount; ++i )
   {
      aiFace const & face = assimpMesh->mFaces[ i ];
      for ( uint32_t j = 0; j < face.mNumIndices; ++j )
      {
         uint32_t index = face.mIndices[ j ];
         outMesh->Indices.emplace_back( index );
      }
   }

   if ( indexCount != uint32_t( outMesh->Indices.size() ) )
   {

      std::cout << "[Warn] " << __FUNCTION__ << " :: Indices differ :: expected(" << indexCount << ", actual(" << outMesh->Indices.size() << ")\n";
   }

   return outMesh;
}

inline irr::scene::ISceneNode*
loadAssimpSceneNode( irr::scene::ISceneManager* smgr, std::string const & fileName )
{
   std::cout << "[Info] " << __FUNCTION__ << " :: fileName(" << fileName << ")\n";

   irrExt::PerfTimer timer;
   timer.start();

   aiScene const * assimpScene = aiImportFile( fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality );
   if ( !assimpScene )
   {
      std::cout << "[Error] " << __FUNCTION__ << " :: Cannot load assimp file(" << fileName << "), "
      << "error(" << aiGetErrorString() << ")\n";
      return nullptr;
   }

   std::string mediaDir = irrExt::getAbsoluteFilePath( fileName );
   std::cout << "[Info] " << __FUNCTION__ << " :: Assimp mediaDir(" << mediaDir << ")\n";

   uint32_t nMeshes = assimpScene->mNumMeshes;
   uint32_t nMaterials = assimpScene->mNumMaterials;

   irr::video::IVideoDriver* driver = smgr->getVideoDriver();
   irr::video::SMaterial outMaterial;

   std::cout << "[Info] " << __FUNCTION__ << " :: Assimp open file(" << fileName << ")\n";
   std::cout << "[Info] " << __FUNCTION__ << " :: Assimp materials(" << nMaterials << ")\n";

   for ( uint32_t i = 0; i < nMaterials; ++i )
   {
      if ( !readMaterial( driver, outMaterial, assimpScene, i, mediaDir ) )
      {
         continue;
      }

   }

   std::cout << "[Info] " << __FUNCTION__ << " :: Assimp meshes(" << nMeshes << ")\n";

   irrExt::AutoSceneNode* node = new irrExt::AutoSceneNode( smgr, smgr->getRootSceneNode(), -1 );

   for ( uint32_t m = 0; m < nMeshes; ++m )
   {
      aiMesh* mesh = assimpScene->mMeshes[ m ];
      if ( !mesh )
      {
         std::cout << "[Warn] " << __FUNCTION__ << " :: Assimp mesh nulltr(" << m << "), skip\n";
         continue;
      }

      uint32_t nVertices = mesh->mNumVertices;
      uint32_t iMaterial = mesh->mMaterialIndex;
      uint32_t nFaces = mesh->mNumFaces;

      std::cout << "[Info] " << __FUNCTION__ << " :: Assimp mesh[" << m << "].vertices(" << nVertices << ")\n";
      std::cout << "[Info] " << __FUNCTION__ << " :: Assimp mesh[" << m << "].material(" << iMaterial << ")\n";
      std::cout << "[Info] " << __FUNCTION__ << " :: Assimp mesh[" << m << "].faces(" << nFaces << ")\n";

      // aiMaterial* mat = assimpScene->mMaterials[ mesh->mMaterialIndex ];
      // aiMaterial* mat = assimpScene->mMaterials[ mesh->mMaterialIndex ];
      // aiColor4D specColor;
      // aiGetMaterialColor( mat, AI_MATKEY_COLOR_SPECULAR/DIFFUSE/AMBIENT, &specCol);
      // aiGetMaterialFloat( mat, AI_MATKEY_SHININESS, &specCol);
      // for mes->mNumVertices
      // aiVector3D ver/norm = mes->mVertices/mNormals
      // for mesh->mNumFaces;
      // indices.pb(mesh->mFaces[f].mIndices[0,1,2]);

      // AutoMeshBuffer* outMesh = new AutoMeshBuffer();

      // outMesh->

      irrExt::AutoMeshBuffer * limb = readMesh( driver, assimpScene, m, mediaDir );
      if ( limb )
      {
         node->addMeshBuffer( limb, true );
      }

   }
   aiReleaseImport( assimpScene );

   std::cout << "[Info] " << __FUNCTION__ << " :: Assimp closed file(" << fileName << ")\n";

   timer.stop();

   std::cout << "[Info] " << __FUNCTION__ << " :: Assimp loader needed(" << timer.asMilliseconds() << ")\n";

   return node;
}

int main( int argc, char * argv[] )
{
   ::srand( static_cast< uint32_t >( ::time( nullptr ) ) );

   const char* appName = "test.assimp (c) 2019 by Benjamin Hampe <benjaminhampe@gmx.de>";

   // if ( argc > 1 )
   // {
      // testModelFileName = argv[1];
   // }

   std::cout << "// ==========================================================\n";
   std::cout << "// prgm-name: " << argv[0] << ":\n";
   std::cout << "// ==========================================================\n";

/// =========================================================
   irrExt::OpenGLDevice oglDevice( 1280, 800, 32 );
   irr::IrrlichtDevice* device = oglDevice.getDevice();
   if ( !device )
   {
      std::cout << "[Error] " << __FUNCTION__ << " :: Cannot create IrrlichtDevice\n";
      return 0;
   }
   device->setResizable( true );
   device->setWindowCaption( irr::core::stringw( appName ).c_str() );

   irrExt::BaseEventReceiver receiver( device );

   irr::video::IVideoDriver* driver = device->getVideoDriver();
   irr::scene::ISceneManager* smgr = device->getSceneManager();
   //irr::scene::ISceneNode* rootNode = smgr->getRootSceneNode();
   irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
   irr::core::dimension2du screenSize = driver->getScreenSize();
   irr::video::SColor clearColor( 255,80,80,200 );

// TEST ASSIMP:
   std::cout << "// ==========================================================\n";
   std::cout << "// TEST_ASSIMP:\n";
   std::cout << "// ==========================================================\n";
   std::string lstFile = irrExt::getAbsoluteFileName( "../../media/test/test.txt" );
   std::string lstDir = irrExt::getAbsoluteFilePath( lstFile );
   std::cout << "lstFile = " << lstFile << "\n";
   std::cout << "lstDir = " << lstDir << "\n";

   std::vector< std::string > models;
   models.push_back( irrExt::getAbsoluteFileName( "../../media/test/3D_Printer_test_fixed_stl_3rd_gen.STL" ) );

   //	= irrExt::getAbsoluteFileNames( irrExt::loadStringVector( lstFile ), lstDir );
   // std::cout << irrExt::toString( models, "models" );

   float32_t modelPosX = 0.0f;
   for ( size_t i = 0; i < models.size(); ++i )
   {
      std::cout << "// ==========================================================\n";
      std::cout << "// TEST[" << i << "] " << models[ i ] << "\n";
      std::cout << "// ==========================================================\n";

      irr::scene::ISceneNode* model = loadAssimpSceneNode( smgr, models[ i ] );
      if ( model )
      {
         model->setPosition( irr::core::vector3df( modelPosX, 0.0f, 0.0f ) );
         model->setMaterialFlag( irr::video::EMF_LIGHTING, true );
         model->setMaterialFlag( irr::video::EMF_WIREFRAME, false );
         model->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );

         irr::core::vector3df modelSize = model->getBoundingBox().getExtent();

         std::cout << "Model Size = " << modelSize << "\n";

//         irr::scene::ISceneNode* clone = model->clone();
//         if ( clone )
//         {
//            clone->setPosition( irr::core::vector3df( modelPosX, 0.0f, 0.0f ) );
//            clone->setScale( irr::core::vector3df( 1.1f, 1.1f, 1.1f ) );
//            clone->setMaterialFlag( irr::video::EMF_LIGHTING, true );
//            clone->setMaterialFlag( irr::video::EMF_WIREFRAME, true );
//            clone->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
//            //clone->setMaterialFlag( irr::video::EMF_GOURAUD_SHADING, true );
//            irr::core::vector3df modelSize = model->getBoundingBox().getExtent();
//         }

         modelPosX += modelSize.X;
      }
   }

   driver->setFog( irr::video::SColor(255, 100, 100, 255 ), irr::video::EFT_FOG_EXP, 500.0f, 1000.0f );

   // addSkyBox( AutoSceneNode* )
//   {
//     irr::video::ITexture* top = Game_getTexture( game, eTexture::SKYBOX_TOP );
//     irr::video::ITexture* bottom = Game_getTexture( game, eTexture::SKYBOX_BOTTOM );
//     irr::video::ITexture* left = Game_getTexture( game, eTexture::SKYBOX_LEFT );
//     irr::video::ITexture* right = Game_getTexture( game, eTexture::SKYBOX_RIGHT );
//     irr::video::ITexture* front = Game_getTexture( game, eTexture::SKYBOX_FRONT );
//     irr::video::ITexture* back = Game_getTexture( game, eTexture::SKYBOX_BACK );
//     //irr::scene::ISceneNode* skyBox =
//     smgr->addSkyBoxSceneNode( top, bottom, left, right, front, back, smgr->getRootSceneNode(), -1 );
//   }

   irr::scene::ICameraSceneNode* cam = irrExt::FpsCamera::create( smgr );
   cam->setNearValue( 1.0f );
   cam->setFarValue( 10000.0f );
   cam->setPosition( irr::core::vector3df( 0.0f, 200.0f, -300.0f ) );
   cam->setTarget( irr::core::vector3df( 0.0f, 0.0f, 0.0f ) );

   irr::core::vector3df l0pos( 0.0f, 1000.0f, 0.0f );
   irr::video::SColorf l0color( 1.0f, 1.0f, .8f, .2f );
   irr::f32 l0radius = 1000.0f;
   irr::s32 l0id = -1;
   irr::scene::ILightSceneNode* l0 = smgr->addLightSceneNode( smgr->getRootSceneNode(), l0pos, l0color, l0radius, l0id );

   irr::core::vector3df l1pos( 0.0f, 0.0f, 0.0f );
   irr::video::SColorf l1color( 1.0f, 1.0f, .8f, .6f );
   irr::f32 l1radius = 500.0f;
   irr::s32 l1id = -1;
   irr::scene::ILightSceneNode* l1 = smgr->addLightSceneNode( smgr->getRootSceneNode(), l1pos, l1color, l1radius, l1id );

   /// MAIN LOOP
   uint64_t timeNow = device->getTimer()->getRealTime();
   uint64_t timeStart = timeNow;
   uint64_t timeLastScreenUpdate = device->getTimer()->getRealTime();
   uint64_t timeLastWindowTitleUpdate = device->getTimer()->getRealTime();
   uint64_t waitUpdateScreen = 0;
   uint64_t waitUpdateWindowTitle = 1000 / 10;

   irr::core::dimension2du lastWindowSize = irr::core::dimension2du( 0,0 );

   while (device && device->run())
   {
      timeNow = device->getTimer()->getRealTime();

      if ( driver )
      {
         screenSize = driver->getScreenSize();
         if ( lastWindowSize != irr::core::dimension2du( 0,0 )
            && screenSize != lastWindowSize )
         {
            float aspect = 1.0f;
            if ( screenSize.Height > 0 )
            {
               aspect = float( screenSize.Width ) / float( screenSize.Height );
            }

            irr::scene::ICameraSceneNode* camera = smgr->getActiveCamera();
            if ( camera )
            {
               camera->setAspectRatio( aspect );
            }
         }
      }

      if ( device->isWindowActive() &&
           timeNow - timeLastScreenUpdate >= waitUpdateScreen )
      {
         timeLastScreenUpdate = timeNow;

         if ( driver )
         {
            driver->beginScene( true, true, clearColor );

            if ( smgr )
            {
               smgr->drawAll();
            }

            if ( guienv )
            {
               guienv->drawAll();
            }

            driver->endScene();

            if ( timeNow - timeLastWindowTitleUpdate >= waitUpdateWindowTitle )
            {
               timeLastWindowTitleUpdate = timeNow;

               std::stringstream s;
               s << appName;
               s << "- FPS(" << driver->getFPS() << ")";
               s << ", Prim(" << device->getVideoDriver()->getPrimitiveCountDrawn() << ")";
               s << ", Tex(" << device->getVideoDriver()->getTextureCount() << ")";
               s << ", Screen(" << screenSize.Width << "," << screenSize.Height << ")";

               irr::scene::ICameraSceneNode* camera = smgr->getActiveCamera();
               if ( camera )
               {
                  irr::core::vector3df const & p = camera->getAbsolutePosition();
                  irr::core::vector3df const & t = camera->getTarget();
                  irr::core::vector3df const & r = camera->getRotation();

                 s << ", CamPos(" << int32_t( p.X ) << ',' << int32_t( p.Y ) << ',' << int32_t( p.Z ) << ")";
                 s << ", CamEye(" << int32_t( t.X ) << ',' << int32_t( t.Y ) << ',' << int32_t( t.Z ) << ")";
                 s << ", CamAng(" << int32_t( r.X ) << ',' << int32_t( r.Y ) << ',' << int32_t( r.Z ) << ")";
               }

               device->setWindowCaption( irr::core::stringw( s.str().c_str() ).c_str() );
            }
         }

      }
      else
      {
         device->yield();
      }

   }
   return 0;
}
