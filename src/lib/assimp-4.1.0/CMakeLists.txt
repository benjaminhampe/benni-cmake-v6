cmake_minimum_required( VERSION 2.8 )

project(benni-lib-assimp CXX)

if (TARGET assimp)
   message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
   set(ASSIMP_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/assimp)
   set(ASSIMP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   set(ASSIMP_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/assimp)

   # All supported options ###############################################
   option(BUILD_SHARED_LIBS "Build package with shared libraries." ON )
   option(BUILD_FRAMEWORK "Build package as Mac OS X Framework bundle." OFF)
   option(ASSIMP_DOUBLE_PRECISION "Set to ON to enable double precision processing" OFF)
   option(ASSIMP_OPT_BUILD_PACKAGES "Set to ON to generate CPack configuration files and packaging targets" OFF)
   option(ASSIMP_ANDROID_JNIIOSYSTEM "Android JNI IOSystem support is active" OFF)
   option(ASSIMP_NO_EXPORT "Disable Assimp's export functionality." OFF)
   option(ASSIMP_BUILD_ZLIB "Build your own zlib" OFF)
   option(ASSIMP_BUILD_ASSIMP_TOOLS "If the supplementary tools for Assimp are built in addition to the library." ON)
   option(ASSIMP_BUILD_SAMPLES "If the official samples are built as well (needs Glut)." OFF)
   option(ASSIMP_BUILD_TESTS "If the test suite for Assimp is built in addition to the library." ON)
   option(ASSIMP_COVERALLS "Enable this to measure test coverage." OFF)
   option(ASSIMP_WERROR "Treat warnings as errors." OFF)
   option(ASSIMP_ASAN "Enable AddressSanitizer." OFF)
   option(ASSIMP_UBSAN "Enable Undefined Behavior sanitizer." OFF)
   option(SYSTEM_IRRXML "Use system installed Irrlicht/IrrXML library." OFF)
   option(BUILD_DOCS "Build documentation using Doxygen." OFF)

   ##### add_definitions(-DASSIMP_DOUBLE_PRECISION)

   # Needed for openddl_parser config, no use of c++11 at this moment
   add_definitions( -DOPENDDL_NO_USE_CPP11 )
   set_property( GLOBAL PROPERTY CXX_STANDARD 11 )

   add_definitions( -DASSIMP_BUILD_NO_C4D_IMPORTER )

   add_definitions( -DASSIMP_BUILD_NO_OWN_ZLIB )

   # add_definitions( -DASSIMP_BUILD_DLL_EXPORT )

   # Define here the needed parameters
   set(LINK_SEARCH_START_STATIC TRUE)
   set(ASSIMP_VERSION_MAJOR 4)
   set(ASSIMP_VERSION_MINOR 1)
   set(ASSIMP_VERSION_PATCH 0)
   set(ASSIMP_VERSION ${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH})
   set(ASSIMP_SOVERSION 4)
   set(PROJECT_VERSION "${ASSIMP_VERSION}")
   set(ASSIMP_PACKAGE_VERSION "0" CACHE STRING "the package-specific version used for uploading the sources" )

   # LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules" )
   # set(LIBASSIMP_COMPONENT     "libassimp${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH}" )
   # set(LIBASSIMP-DEV_COMPONENT "libassimp${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH}-dev" )
   # set(CPACK_COMPONENTS_ALL assimp-bin ${LIBASSIMP_COMPONENT} ${LIBASSIMP-DEV_COMPONENT} assimp-dev)
   set(ASSIMP_LIBRARY_SUFFIX "" CACHE STRING "Suffix to append to library names")

   # Get the current working branch
   EXECUTE_PROCESS(
     COMMAND git rev-parse --abbrev-ref HEAD
     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
     OUTPUT_VARIABLE GIT_BRANCH
     OUTPUT_STRIP_TRAILING_WHITESPACE
     ERROR_QUIET
   )

   # Get the latest abbreviated commit hash of the working branch
   EXECUTE_PROCESS(
     COMMAND git log -1 --format=%h
     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
     OUTPUT_VARIABLE GIT_COMMIT_HASH
     OUTPUT_STRIP_TRAILING_WHITESPACE
     ERROR_QUIET
   )

   IF (NOT GIT_COMMIT_HASH)
      set(GIT_COMMIT_HASH 0)
   ENDIF()

   CONFIGURE_FILE(
     ${CMAKE_CURRENT_LIST_DIR}/revision.h.in
     ${ASSIMP_INCLUDE_DIR}/revision.h
   )

   CONFIGURE_FILE(
     ${ASSIMP_HEADER_DIR}/config.h.in
     ${ASSIMP_INCLUDE_DIR}/config.h
   )

   set(ASSIMP_HEADERS_II
      ${ASSIMP_HEADER_DIR}/Compiler/pushpack1.h
      ${ASSIMP_HEADER_DIR}/Compiler/poppack1.h
      ${ASSIMP_HEADER_DIR}/Compiler/pstdint.h)

   set(ASSIMP_HEADERS
      ${ASSIMP_HEADER_DIR}/anim.h
      ${ASSIMP_HEADER_DIR}/ai_assert.h
      ${ASSIMP_HEADER_DIR}/camera.h
      ${ASSIMP_HEADER_DIR}/color4.h
      ${ASSIMP_HEADER_DIR}/color4.inl
      ${ASSIMP_INCLUDE_DIR}/config.h
      ${ASSIMP_HEADER_DIR}/defs.h
      ${ASSIMP_HEADER_DIR}/Defines.h
      ${ASSIMP_HEADER_DIR}/cfileio.h
      ${ASSIMP_HEADER_DIR}/light.h
      ${ASSIMP_HEADER_DIR}/material.h
      ${ASSIMP_HEADER_DIR}/material.inl
      ${ASSIMP_HEADER_DIR}/matrix3x3.h
      ${ASSIMP_HEADER_DIR}/matrix3x3.inl
      ${ASSIMP_HEADER_DIR}/matrix4x4.h
      ${ASSIMP_HEADER_DIR}/matrix4x4.inl
      ${ASSIMP_HEADER_DIR}/mesh.h
      ${ASSIMP_HEADER_DIR}/postprocess.h
      ${ASSIMP_HEADER_DIR}/quaternion.h
      ${ASSIMP_HEADER_DIR}/quaternion.inl
      ${ASSIMP_HEADER_DIR}/scene.h
      ${ASSIMP_HEADER_DIR}/metadata.h
      ${ASSIMP_HEADER_DIR}/texture.h
      ${ASSIMP_HEADER_DIR}/types.h
      ${ASSIMP_HEADER_DIR}/vector2.h
      ${ASSIMP_HEADER_DIR}/vector2.inl
      ${ASSIMP_HEADER_DIR}/vector3.h
      ${ASSIMP_HEADER_DIR}/vector3.inl
      ${ASSIMP_HEADER_DIR}/version.h
      ${ASSIMP_HEADER_DIR}/cimport.h
      ${ASSIMP_HEADER_DIR}/importerdesc.h
      ${ASSIMP_HEADER_DIR}/Importer.hpp
      ${ASSIMP_HEADER_DIR}/DefaultLogger.hpp
      ${ASSIMP_HEADER_DIR}/ProgressHandler.hpp
      ${ASSIMP_HEADER_DIR}/IOStream.hpp
      ${ASSIMP_HEADER_DIR}/IOSystem.hpp
      ${ASSIMP_HEADER_DIR}/Logger.hpp
      ${ASSIMP_HEADER_DIR}/LogStream.hpp
      ${ASSIMP_HEADER_DIR}/NullLogger.hpp
      ${ASSIMP_HEADER_DIR}/cexport.h
      ${ASSIMP_HEADER_DIR}/Exporter.hpp
      ${ASSIMP_HEADER_DIR}/DefaultIOStream.h
      ${ASSIMP_HEADER_DIR}/DefaultIOSystem.h
      ${ASSIMP_HEADER_DIR}/SceneCombiner.h
   )

   set(ASSIMP_SOURCES
   # Core_SRCS
      ${ASSIMP_SOURCE_DIR}/Assimp.cpp
   # Logging_SRCS
      ${ASSIMP_HEADER_DIR}/DefaultLogger.hpp
      ${ASSIMP_HEADER_DIR}/LogStream.hpp
      ${ASSIMP_HEADER_DIR}/Logger.hpp
      ${ASSIMP_HEADER_DIR}/NullLogger.hpp
      ${ASSIMP_SOURCE_DIR}/Win32DebugLogStream.h
      ${ASSIMP_SOURCE_DIR}/DefaultLogger.cpp
      ${ASSIMP_SOURCE_DIR}/FileLogStream.h
      ${ASSIMP_SOURCE_DIR}/StdOStreamLogStream.h
      ${ASSIMP_SOURCE_DIR}/irrXMLWrapper.h
   # Common_SRCS
      ${ASSIMP_SOURCE_DIR}/fast_atof.h
      ${ASSIMP_SOURCE_DIR}/qnan.h
      ${ASSIMP_SOURCE_DIR}/BaseImporter.cpp
      ${ASSIMP_SOURCE_DIR}/BaseImporter.h
      ${ASSIMP_SOURCE_DIR}/BaseProcess.cpp
      ${ASSIMP_SOURCE_DIR}/BaseProcess.h
      ${ASSIMP_SOURCE_DIR}/Importer.h
      ${ASSIMP_SOURCE_DIR}/ScenePrivate.h
      ${ASSIMP_SOURCE_DIR}/PostStepRegistry.cpp
      ${ASSIMP_SOURCE_DIR}/ImporterRegistry.cpp
      ${ASSIMP_SOURCE_DIR}/ByteSwapper.h
      ${ASSIMP_SOURCE_DIR}/DefaultProgressHandler.h
      ${ASSIMP_SOURCE_DIR}/DefaultIOStream.cpp
      ${ASSIMP_SOURCE_DIR}/DefaultIOSystem.cpp
      ${ASSIMP_SOURCE_DIR}/CInterfaceIOWrapper.cpp
      ${ASSIMP_SOURCE_DIR}/CInterfaceIOWrapper.h
      ${ASSIMP_SOURCE_DIR}/Hash.h
      ${ASSIMP_SOURCE_DIR}/Importer.cpp
      ${ASSIMP_SOURCE_DIR}/IFF.h
      ${ASSIMP_SOURCE_DIR}/MemoryIOWrapper.h
      ${ASSIMP_SOURCE_DIR}/ParsingUtils.h
      ${ASSIMP_SOURCE_DIR}/StreamReader.h
      ${ASSIMP_SOURCE_DIR}/StreamWriter.h
      ${ASSIMP_SOURCE_DIR}/StringComparison.h
      ${ASSIMP_SOURCE_DIR}/StringUtils.h
      ${ASSIMP_SOURCE_DIR}/SGSpatialSort.cpp
      ${ASSIMP_SOURCE_DIR}/SGSpatialSort.h
      ${ASSIMP_SOURCE_DIR}/VertexTriangleAdjacency.cpp
      ${ASSIMP_SOURCE_DIR}/VertexTriangleAdjacency.h
      ${ASSIMP_SOURCE_DIR}/GenericProperty.h
      ${ASSIMP_SOURCE_DIR}/SpatialSort.cpp
      ${ASSIMP_SOURCE_DIR}/SpatialSort.h
      ${ASSIMP_SOURCE_DIR}/SceneCombiner.cpp
      ${ASSIMP_SOURCE_DIR}/ScenePreprocessor.cpp
      ${ASSIMP_SOURCE_DIR}/ScenePreprocessor.h
      ${ASSIMP_SOURCE_DIR}/SkeletonMeshBuilder.cpp
      ${ASSIMP_SOURCE_DIR}/SkeletonMeshBuilder.h
      ${ASSIMP_SOURCE_DIR}/SplitByBoneCountProcess.cpp
      ${ASSIMP_SOURCE_DIR}/SplitByBoneCountProcess.h
      ${ASSIMP_SOURCE_DIR}/ScaleProcess.cpp
      ${ASSIMP_SOURCE_DIR}/ScaleProcess.h
      ${ASSIMP_SOURCE_DIR}/SmoothingGroups.h
      ${ASSIMP_SOURCE_DIR}/StandardShapes.cpp
      ${ASSIMP_SOURCE_DIR}/StandardShapes.h
      ${ASSIMP_SOURCE_DIR}/TargetAnimation.cpp
      ${ASSIMP_SOURCE_DIR}/TargetAnimation.h
      ${ASSIMP_SOURCE_DIR}/RemoveComments.cpp
      ${ASSIMP_SOURCE_DIR}/RemoveComments.h
      ${ASSIMP_SOURCE_DIR}/Subdivision.cpp
      ${ASSIMP_SOURCE_DIR}/Subdivision.h
      ${ASSIMP_SOURCE_DIR}/scene.cpp
      ${ASSIMP_SOURCE_DIR}/Vertex.h
      ${ASSIMP_SOURCE_DIR}/LineSplitter.h
      ${ASSIMP_SOURCE_DIR}/TinyFormatter.h
      ${ASSIMP_SOURCE_DIR}/Profiler.h
      ${ASSIMP_SOURCE_DIR}/LogAux.h
      ${ASSIMP_SOURCE_DIR}/Bitmap.cpp
      ${ASSIMP_SOURCE_DIR}/Bitmap.h
      ${ASSIMP_SOURCE_DIR}/XMLTools.h
      ${ASSIMP_SOURCE_DIR}/Version.cpp
      ${ASSIMP_SOURCE_DIR}/IOStreamBuffer.h
      ${ASSIMP_SOURCE_DIR}/CreateAnimMesh.h
      ${ASSIMP_SOURCE_DIR}/CreateAnimMesh.cpp
      # Non Free C4D sources
      ${ASSIMP_SOURCE_DIR}/C4DImporter.cpp
      ${ASSIMP_SOURCE_DIR}/C4DImporter.h
      # AMF
      ${ASSIMP_SOURCE_DIR}/AMFImporter.hpp
      ${ASSIMP_SOURCE_DIR}/AMFImporter_Macro.hpp
      ${ASSIMP_SOURCE_DIR}/AMFImporter_Node.hpp
      ${ASSIMP_SOURCE_DIR}/AMFImporter.cpp
      ${ASSIMP_SOURCE_DIR}/AMFImporter_Geometry.cpp
      ${ASSIMP_SOURCE_DIR}/AMFImporter_Material.cpp
      ${ASSIMP_SOURCE_DIR}/AMFImporter_Postprocess.cpp
      # 3DS
      ${ASSIMP_SOURCE_DIR}/3DSConverter.cpp
      ${ASSIMP_SOURCE_DIR}/3DSHelper.h
      ${ASSIMP_SOURCE_DIR}/3DSLoader.cpp
      ${ASSIMP_SOURCE_DIR}/3DSLoader.h
      ${ASSIMP_SOURCE_DIR}/3DSExporter.h
      ${ASSIMP_SOURCE_DIR}/3DSExporter.cpp
      # AC
      ${ASSIMP_SOURCE_DIR}/ACLoader.cpp
      ${ASSIMP_SOURCE_DIR}/ACLoader.h
      # ASE
      ${ASSIMP_SOURCE_DIR}/ASELoader.cpp
      ${ASSIMP_SOURCE_DIR}/ASELoader.h
      ${ASSIMP_SOURCE_DIR}/ASEParser.cpp
      ${ASSIMP_SOURCE_DIR}/ASEParser.h
      # ASSBIN
      ${ASSIMP_SOURCE_DIR}/AssbinExporter.h
      ${ASSIMP_SOURCE_DIR}/AssbinExporter.cpp
      ${ASSIMP_SOURCE_DIR}/AssbinLoader.h
      ${ASSIMP_SOURCE_DIR}/AssbinLoader.cpp
      # ASSXML
      ${ASSIMP_SOURCE_DIR}/AssxmlExporter.h
      ${ASSIMP_SOURCE_DIR}/AssxmlExporter.cpp
      # B3D
      ${ASSIMP_SOURCE_DIR}/B3DImporter.cpp
      ${ASSIMP_SOURCE_DIR}/B3DImporter.h
      # BVH
      ${ASSIMP_SOURCE_DIR}/BVHLoader.cpp
      ${ASSIMP_SOURCE_DIR}/BVHLoader.h
      # COLLADA
      ${ASSIMP_SOURCE_DIR}/ColladaHelper.h
      ${ASSIMP_SOURCE_DIR}/ColladaLoader.cpp
      ${ASSIMP_SOURCE_DIR}/ColladaLoader.h
      ${ASSIMP_SOURCE_DIR}/ColladaParser.cpp
      ${ASSIMP_SOURCE_DIR}/ColladaParser.h
      ${ASSIMP_SOURCE_DIR}/ColladaExporter.h
      ${ASSIMP_SOURCE_DIR}/ColladaExporter.cpp
      # DXF
      ${ASSIMP_SOURCE_DIR}/DXFLoader.cpp
      ${ASSIMP_SOURCE_DIR}/DXFLoader.h
      ${ASSIMP_SOURCE_DIR}/DXFHelper.h
      # CSM
      ${ASSIMP_SOURCE_DIR}/CSMLoader.cpp
      ${ASSIMP_SOURCE_DIR}/CSMLoader.h
      # HMP
      ${ASSIMP_SOURCE_DIR}/HMPFileData.h
      ${ASSIMP_SOURCE_DIR}/HMPLoader.cpp
      ${ASSIMP_SOURCE_DIR}/HMPLoader.h
      ${ASSIMP_SOURCE_DIR}/HalfLifeFileData.h
      # IRRMESH
      ${ASSIMP_SOURCE_DIR}/IRRMeshLoader.cpp
      ${ASSIMP_SOURCE_DIR}/IRRMeshLoader.h
      ${ASSIMP_SOURCE_DIR}/IRRShared.cpp
      ${ASSIMP_SOURCE_DIR}/IRRShared.h
      # IRR
      ${ASSIMP_SOURCE_DIR}/IRRLoader.cpp
      ${ASSIMP_SOURCE_DIR}/IRRLoader.h
      ${ASSIMP_SOURCE_DIR}/IRRShared.cpp
      ${ASSIMP_SOURCE_DIR}/IRRShared.h
      # LWO
      ${ASSIMP_SOURCE_DIR}/LWOAnimation.cpp
      ${ASSIMP_SOURCE_DIR}/LWOAnimation.h
      ${ASSIMP_SOURCE_DIR}/LWOBLoader.cpp
      ${ASSIMP_SOURCE_DIR}/LWOFileData.h
      ${ASSIMP_SOURCE_DIR}/LWOLoader.cpp
      ${ASSIMP_SOURCE_DIR}/LWOLoader.h
      ${ASSIMP_SOURCE_DIR}/LWOMaterial.cpp
      # LWS
      ${ASSIMP_SOURCE_DIR}/LWSLoader.cpp
      ${ASSIMP_SOURCE_DIR}/LWSLoader.h
      # MD2
      ${ASSIMP_SOURCE_DIR}/MD2FileData.h
      ${ASSIMP_SOURCE_DIR}/MD2Loader.cpp
      ${ASSIMP_SOURCE_DIR}/MD2Loader.h
      ${ASSIMP_SOURCE_DIR}/MD2NormalTable.h
      # MD3
      ${ASSIMP_SOURCE_DIR}/MD3FileData.h
      ${ASSIMP_SOURCE_DIR}/MD3Loader.cpp
      ${ASSIMP_SOURCE_DIR}/MD3Loader.h
      # MD5
      ${ASSIMP_SOURCE_DIR}/MD5Loader.cpp
      ${ASSIMP_SOURCE_DIR}/MD5Loader.h
      ${ASSIMP_SOURCE_DIR}/MD5Parser.cpp
      ${ASSIMP_SOURCE_DIR}/MD5Parser.h
      # MDC
      ${ASSIMP_SOURCE_DIR}/MDCFileData.h
      ${ASSIMP_SOURCE_DIR}/MDCLoader.cpp
      ${ASSIMP_SOURCE_DIR}/MDCLoader.h
      ${ASSIMP_SOURCE_DIR}/MDCNormalTable.h
      # MDL
      ${ASSIMP_SOURCE_DIR}/MDLDefaultColorMap.h
      ${ASSIMP_SOURCE_DIR}/MDLFileData.h
      ${ASSIMP_SOURCE_DIR}/MDLLoader.cpp
      ${ASSIMP_SOURCE_DIR}/MDLLoader.h
      ${ASSIMP_SOURCE_DIR}/MDLMaterialLoader.cpp
      # MaterialSystem_SRCS
      ${ASSIMP_SOURCE_DIR}/MaterialSystem.cpp
      ${ASSIMP_SOURCE_DIR}/MaterialSystem.h
      # NFF
      ${ASSIMP_SOURCE_DIR}/NFFLoader.cpp
      ${ASSIMP_SOURCE_DIR}/NFFLoader.h
      # NDO
      ${ASSIMP_SOURCE_DIR}/NDOLoader.cpp
      ${ASSIMP_SOURCE_DIR}/NDOLoader.h
      # OFF
      ${ASSIMP_SOURCE_DIR}/OFFLoader.cpp
      ${ASSIMP_SOURCE_DIR}/OFFLoader.h
      # OBJ
      ${ASSIMP_SOURCE_DIR}/ObjFileData.h
      ${ASSIMP_SOURCE_DIR}/ObjFileImporter.cpp
      ${ASSIMP_SOURCE_DIR}/ObjFileImporter.h
      ${ASSIMP_SOURCE_DIR}/ObjFileMtlImporter.cpp
      ${ASSIMP_SOURCE_DIR}/ObjFileMtlImporter.h
      ${ASSIMP_SOURCE_DIR}/ObjFileParser.cpp
      ${ASSIMP_SOURCE_DIR}/ObjFileParser.h
      ${ASSIMP_SOURCE_DIR}/ObjTools.h
      ${ASSIMP_SOURCE_DIR}/ObjExporter.h
      ${ASSIMP_SOURCE_DIR}/ObjExporter.cpp
      # OGRE
      ${ASSIMP_SOURCE_DIR}/OgreImporter.h
      ${ASSIMP_SOURCE_DIR}/OgreStructs.h
      ${ASSIMP_SOURCE_DIR}/OgreParsingUtils.h
      ${ASSIMP_SOURCE_DIR}/OgreBinarySerializer.h
      ${ASSIMP_SOURCE_DIR}/OgreXmlSerializer.h
      ${ASSIMP_SOURCE_DIR}/OgreImporter.cpp
      ${ASSIMP_SOURCE_DIR}/OgreStructs.cpp
      ${ASSIMP_SOURCE_DIR}/OgreBinarySerializer.cpp
      ${ASSIMP_SOURCE_DIR}/OgreXmlSerializer.cpp
      ${ASSIMP_SOURCE_DIR}/OgreMaterial.cpp
      # OPENGEX
      ${ASSIMP_SOURCE_DIR}/OpenGEXExporter.cpp
      ${ASSIMP_SOURCE_DIR}/OpenGEXExporter.h
      ${ASSIMP_SOURCE_DIR}/OpenGEXImporter.cpp
      ${ASSIMP_SOURCE_DIR}/OpenGEXImporter.h
      ${ASSIMP_SOURCE_DIR}/OpenGEXStructs.h
      # PLY
      ${ASSIMP_SOURCE_DIR}/PlyLoader.cpp
      ${ASSIMP_SOURCE_DIR}/PlyLoader.h
      ${ASSIMP_SOURCE_DIR}/PlyParser.cpp
      ${ASSIMP_SOURCE_DIR}/PlyParser.h
      ${ASSIMP_SOURCE_DIR}/PlyExporter.cpp
      ${ASSIMP_SOURCE_DIR}/PlyExporter.h
      # MS3D
      ${ASSIMP_SOURCE_DIR}/MS3DLoader.cpp
      ${ASSIMP_SOURCE_DIR}/MS3DLoader.h
      # COB
      ${ASSIMP_SOURCE_DIR}/COBLoader.cpp
      ${ASSIMP_SOURCE_DIR}/COBLoader.h
      ${ASSIMP_SOURCE_DIR}/COBScene.h
      # BLEND
      ${ASSIMP_SOURCE_DIR}/BlenderLoader.cpp
      ${ASSIMP_SOURCE_DIR}/BlenderLoader.h
      ${ASSIMP_SOURCE_DIR}/BlenderDNA.cpp
      ${ASSIMP_SOURCE_DIR}/BlenderDNA.h
      ${ASSIMP_SOURCE_DIR}/BlenderDNA.inl
      ${ASSIMP_SOURCE_DIR}/BlenderScene.cpp
      ${ASSIMP_SOURCE_DIR}/BlenderScene.h
      ${ASSIMP_SOURCE_DIR}/BlenderSceneGen.h
      ${ASSIMP_SOURCE_DIR}/BlenderIntermediate.h
      ${ASSIMP_SOURCE_DIR}/BlenderModifier.h
      ${ASSIMP_SOURCE_DIR}/BlenderModifier.cpp
      ${ASSIMP_SOURCE_DIR}/BlenderBMesh.h
      ${ASSIMP_SOURCE_DIR}/BlenderBMesh.cpp
      ${ASSIMP_SOURCE_DIR}/BlenderTessellator.h
      ${ASSIMP_SOURCE_DIR}/BlenderTessellator.cpp
      # IFC
      ${ASSIMP_SOURCE_DIR}/IFCLoader.cpp
      ${ASSIMP_SOURCE_DIR}/IFCLoader.h
      ${ASSIMP_SOURCE_DIR}/IFCReaderGen1.cpp
      ${ASSIMP_SOURCE_DIR}/IFCReaderGen2.cpp
      ${ASSIMP_SOURCE_DIR}/IFCReaderGen.h
      ${ASSIMP_SOURCE_DIR}/IFCUtil.h
      ${ASSIMP_SOURCE_DIR}/IFCUtil.cpp
      ${ASSIMP_SOURCE_DIR}/IFCGeometry.cpp
      ${ASSIMP_SOURCE_DIR}/IFCMaterial.cpp
      ${ASSIMP_SOURCE_DIR}/IFCProfile.cpp
      ${ASSIMP_SOURCE_DIR}/IFCCurve.cpp
      ${ASSIMP_SOURCE_DIR}/IFCBoolean.cpp
      ${ASSIMP_SOURCE_DIR}/IFCOpenings.cpp
      ${ASSIMP_SOURCE_DIR}/STEPFile.h
      ${ASSIMP_SOURCE_DIR}/STEPFileReader.h
      ${ASSIMP_SOURCE_DIR}/STEPFileReader.cpp
      ${ASSIMP_SOURCE_DIR}/STEPFileEncoding.cpp
      ${ASSIMP_SOURCE_DIR}/STEPFileEncoding.h
      # XGL
      ${ASSIMP_SOURCE_DIR}/XGLLoader.cpp
      ${ASSIMP_SOURCE_DIR}/XGLLoader.h
      # FBX
      ${ASSIMP_SOURCE_DIR}/FBXImporter.cpp
      ${ASSIMP_SOURCE_DIR}/FBXCompileConfig.h
      ${ASSIMP_SOURCE_DIR}/FBXImporter.h
      ${ASSIMP_SOURCE_DIR}/FBXParser.cpp
      ${ASSIMP_SOURCE_DIR}/FBXParser.h
      ${ASSIMP_SOURCE_DIR}/FBXTokenizer.cpp
      ${ASSIMP_SOURCE_DIR}/FBXTokenizer.h
      ${ASSIMP_SOURCE_DIR}/FBXImportSettings.h
      ${ASSIMP_SOURCE_DIR}/FBXConverter.h
      ${ASSIMP_SOURCE_DIR}/FBXConverter.cpp
      ${ASSIMP_SOURCE_DIR}/FBXUtil.h
      ${ASSIMP_SOURCE_DIR}/FBXUtil.cpp
      ${ASSIMP_SOURCE_DIR}/FBXDocument.h
      ${ASSIMP_SOURCE_DIR}/FBXDocument.cpp
      ${ASSIMP_SOURCE_DIR}/FBXProperties.h
      ${ASSIMP_SOURCE_DIR}/FBXProperties.cpp
      ${ASSIMP_SOURCE_DIR}/FBXMeshGeometry.h
      ${ASSIMP_SOURCE_DIR}/FBXMeshGeometry.cpp
      ${ASSIMP_SOURCE_DIR}/FBXMaterial.cpp
      ${ASSIMP_SOURCE_DIR}/FBXModel.cpp
      ${ASSIMP_SOURCE_DIR}/FBXAnimation.cpp
      ${ASSIMP_SOURCE_DIR}/FBXNodeAttribute.cpp
      ${ASSIMP_SOURCE_DIR}/FBXDeformer.cpp
      ${ASSIMP_SOURCE_DIR}/FBXBinaryTokenizer.cpp
      ${ASSIMP_SOURCE_DIR}/FBXDocumentUtil.cpp
      # PostProcessing_SRCS
      ${ASSIMP_SOURCE_DIR}/CalcTangentsProcess.cpp
      ${ASSIMP_SOURCE_DIR}/CalcTangentsProcess.h
      ${ASSIMP_SOURCE_DIR}/ComputeUVMappingProcess.cpp
      ${ASSIMP_SOURCE_DIR}/ComputeUVMappingProcess.h
      ${ASSIMP_SOURCE_DIR}/ConvertToLHProcess.cpp
      ${ASSIMP_SOURCE_DIR}/ConvertToLHProcess.h
      ${ASSIMP_SOURCE_DIR}/FindDegenerates.cpp
      ${ASSIMP_SOURCE_DIR}/FindDegenerates.h
      ${ASSIMP_SOURCE_DIR}/FindInstancesProcess.cpp
      ${ASSIMP_SOURCE_DIR}/FindInstancesProcess.h
      ${ASSIMP_SOURCE_DIR}/FindInvalidDataProcess.cpp
      ${ASSIMP_SOURCE_DIR}/FindInvalidDataProcess.h
      ${ASSIMP_SOURCE_DIR}/FixNormalsStep.cpp
      ${ASSIMP_SOURCE_DIR}/FixNormalsStep.h
      ${ASSIMP_SOURCE_DIR}/GenFaceNormalsProcess.cpp
      ${ASSIMP_SOURCE_DIR}/GenFaceNormalsProcess.h
      ${ASSIMP_SOURCE_DIR}/GenVertexNormalsProcess.cpp
      ${ASSIMP_SOURCE_DIR}/GenVertexNormalsProcess.h
      ${ASSIMP_SOURCE_DIR}/PretransformVertices.cpp
      ${ASSIMP_SOURCE_DIR}/PretransformVertices.h
      ${ASSIMP_SOURCE_DIR}/ImproveCacheLocality.cpp
      ${ASSIMP_SOURCE_DIR}/ImproveCacheLocality.h
      ${ASSIMP_SOURCE_DIR}/JoinVerticesProcess.cpp
      ${ASSIMP_SOURCE_DIR}/JoinVerticesProcess.h
      ${ASSIMP_SOURCE_DIR}/LimitBoneWeightsProcess.cpp
      ${ASSIMP_SOURCE_DIR}/LimitBoneWeightsProcess.h
      ${ASSIMP_SOURCE_DIR}/RemoveRedundantMaterials.cpp
      ${ASSIMP_SOURCE_DIR}/RemoveRedundantMaterials.h
      ${ASSIMP_SOURCE_DIR}/RemoveVCProcess.cpp
      ${ASSIMP_SOURCE_DIR}/RemoveVCProcess.h
      ${ASSIMP_SOURCE_DIR}/SortByPTypeProcess.cpp
      ${ASSIMP_SOURCE_DIR}/SortByPTypeProcess.h
      ${ASSIMP_SOURCE_DIR}/SplitLargeMeshes.cpp
      ${ASSIMP_SOURCE_DIR}/SplitLargeMeshes.h
      ${ASSIMP_SOURCE_DIR}/TextureTransform.cpp
      ${ASSIMP_SOURCE_DIR}/TextureTransform.h
      ${ASSIMP_SOURCE_DIR}/TriangulateProcess.cpp
      ${ASSIMP_SOURCE_DIR}/TriangulateProcess.h
      ${ASSIMP_SOURCE_DIR}/ValidateDataStructure.cpp
      ${ASSIMP_SOURCE_DIR}/ValidateDataStructure.h
      ${ASSIMP_SOURCE_DIR}/OptimizeGraph.cpp
      ${ASSIMP_SOURCE_DIR}/OptimizeGraph.h
      ${ASSIMP_SOURCE_DIR}/OptimizeMeshes.cpp
      ${ASSIMP_SOURCE_DIR}/OptimizeMeshes.h
      ${ASSIMP_SOURCE_DIR}/DeboneProcess.cpp
      ${ASSIMP_SOURCE_DIR}/DeboneProcess.h
      ${ASSIMP_SOURCE_DIR}/ProcessHelper.h
      ${ASSIMP_SOURCE_DIR}/ProcessHelper.cpp
      ${ASSIMP_SOURCE_DIR}/PolyTools.h
      ${ASSIMP_SOURCE_DIR}/MakeVerboseFormat.cpp
      ${ASSIMP_SOURCE_DIR}/MakeVerboseFormat.h
      # Q3D
      ${ASSIMP_SOURCE_DIR}/Q3DLoader.cpp
      ${ASSIMP_SOURCE_DIR}/Q3DLoader.h
      # Q3BSP
      ${ASSIMP_SOURCE_DIR}/Q3BSPFileData.h
      ${ASSIMP_SOURCE_DIR}/Q3BSPFileParser.h
      ${ASSIMP_SOURCE_DIR}/Q3BSPFileParser.cpp
      ${ASSIMP_SOURCE_DIR}/Q3BSPFileImporter.h
      ${ASSIMP_SOURCE_DIR}/Q3BSPFileImporter.cpp
      ${ASSIMP_SOURCE_DIR}/Q3BSPZipArchive.h
      ${ASSIMP_SOURCE_DIR}/Q3BSPZipArchive.cpp
      # RAW
      ${ASSIMP_SOURCE_DIR}/RawLoader.cpp
      ${ASSIMP_SOURCE_DIR}/RawLoader.h
      # SIB
      ${ASSIMP_SOURCE_DIR}/SIBImporter.cpp
      ${ASSIMP_SOURCE_DIR}/SIBImporter.h
      # SMD
      ${ASSIMP_SOURCE_DIR}/SMDLoader.cpp
      ${ASSIMP_SOURCE_DIR}/SMDLoader.h
      # STL
      ${ASSIMP_SOURCE_DIR}/STLLoader.cpp
      ${ASSIMP_SOURCE_DIR}/STLLoader.h
      ${ASSIMP_SOURCE_DIR}/STLExporter.h
      ${ASSIMP_SOURCE_DIR}/STLExporter.cpp
      # TERRAGEN
      ${ASSIMP_SOURCE_DIR}/TerragenLoader.cpp
      ${ASSIMP_SOURCE_DIR}/TerragenLoader.h
      # 3D
      ${ASSIMP_SOURCE_DIR}/UnrealLoader.cpp
      ${ASSIMP_SOURCE_DIR}/UnrealLoader.h
      # X
      ${ASSIMP_SOURCE_DIR}/XFileHelper.h
      ${ASSIMP_SOURCE_DIR}/XFileImporter.cpp
      ${ASSIMP_SOURCE_DIR}/XFileImporter.h
      ${ASSIMP_SOURCE_DIR}/XFileParser.cpp
      ${ASSIMP_SOURCE_DIR}/XFileParser.h
      ${ASSIMP_SOURCE_DIR}/XFileExporter.h
      ${ASSIMP_SOURCE_DIR}/XFileExporter.cpp
      # X3D
      ${ASSIMP_SOURCE_DIR}/X3DExporter.cpp
      ${ASSIMP_SOURCE_DIR}/X3DExporter.hpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter.hpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Geometry2D.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Geometry3D.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Group.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Light.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Macro.hpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Metadata.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Networking.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Node.hpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Postprocess.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Rendering.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Shape.cpp
      ${ASSIMP_SOURCE_DIR}/X3DImporter_Texturing.cpp
      ${ASSIMP_SOURCE_DIR}/FIReader.hpp
      ${ASSIMP_SOURCE_DIR}/FIReader.cpp
      ${ASSIMP_SOURCE_DIR}/X3DVocabulary.cpp
      # GLTF
      ${ASSIMP_SOURCE_DIR}/glTFAsset.h
      ${ASSIMP_SOURCE_DIR}/glTFAsset.inl
      ${ASSIMP_SOURCE_DIR}/glTFAssetWriter.h
      ${ASSIMP_SOURCE_DIR}/glTFAssetWriter.inl
      ${ASSIMP_SOURCE_DIR}/glTFImporter.cpp
      ${ASSIMP_SOURCE_DIR}/glTFImporter.h
      ${ASSIMP_SOURCE_DIR}/glTFExporter.h
      ${ASSIMP_SOURCE_DIR}/glTFExporter.cpp
      ${ASSIMP_SOURCE_DIR}/glTF2Asset.h
      ${ASSIMP_SOURCE_DIR}/glTF2Asset.inl
      ${ASSIMP_SOURCE_DIR}/glTF2AssetWriter.h
      ${ASSIMP_SOURCE_DIR}/glTF2AssetWriter.inl
      ${ASSIMP_SOURCE_DIR}/glTF2Importer.cpp
      ${ASSIMP_SOURCE_DIR}/glTF2Importer.h
      ${ASSIMP_SOURCE_DIR}/glTF2Exporter.h
      ${ASSIMP_SOURCE_DIR}/glTF2Exporter.cpp
      # 3MF
      ${ASSIMP_SOURCE_DIR}/D3MFImporter.h
      ${ASSIMP_SOURCE_DIR}/D3MFImporter.cpp
      ${ASSIMP_SOURCE_DIR}/D3MFExporter.h
      ${ASSIMP_SOURCE_DIR}/D3MFExporter.cpp
      ${ASSIMP_SOURCE_DIR}/D3MFOpcPackage.h
      ${ASSIMP_SOURCE_DIR}/D3MFOpcPackage.cpp
      ${ASSIMP_SOURCE_DIR}/3MFXmlTags.h
      # MMD
      ${ASSIMP_SOURCE_DIR}/MMDCpp14.h
      ${ASSIMP_SOURCE_DIR}/MMDImporter.cpp
      ${ASSIMP_SOURCE_DIR}/MMDImporter.h
      ${ASSIMP_SOURCE_DIR}/MMDPmdParser.h
      ${ASSIMP_SOURCE_DIR}/MMDPmxParser.h
      ${ASSIMP_SOURCE_DIR}/MMDPmxParser.cpp
      ${ASSIMP_SOURCE_DIR}/MMDVmdParser.h
      # Step_SRCS
      ${ASSIMP_SOURCE_DIR}/StepExporter.h
      ${ASSIMP_SOURCE_DIR}/StepExporter.cpp
      # Exporter_SRCS
      ${ASSIMP_SOURCE_DIR}/Exporter.cpp
      ${ASSIMP_SOURCE_DIR}/AssimpCExport.cpp
      ${ASSIMP_SOURCE_DIR}/BlobIOSystem.h
      # Extra_SRCS
      ${ASSIMP_SOURCE_DIR}/MD4FileData.h
   )

	if (BENNI_SDK_SHARED)
		message("${Blue}SHARED_LIBRARY: ${PROJECT_NAME}${ColourReset}")
		add_library(assimp SHARED "")	
	else()
		message("${Blue}STATIC_LIBRARY: ${PROJECT_NAME}${ColourReset}")
		add_library(assimp STATIC "")
	endif()
	
   target_include_directories(assimp PRIVATE
      ${ASSIMP_HEADER_DIR}
      ${ASSIMP_HEADER_DIR}/..
      ${ASSIMP_SOURCE_DIR})
   target_sources(assimp PRIVATE ${ASSIMP_SOURCES} ${ASSIMP_HEADERS} ${ASSIMP_HEADERS_II})

   # if (ASSIMP_BUILD_IFC_IMPORTER)
   	if (MSVC)
   		set_source_files_properties(${ASSIMP_SOURCE_DIR}/IFCReaderGen1.cpp ${ASSIMP_SOURCE_DIR}/IFCReaderGen2.cpp PROPERTIES COMPILE_FLAGS "/bigobj")
   	elseif(CMAKE_COMPILER_IS_MINGW)
   		set_source_files_properties(${ASSIMP_SOURCE_DIR}/IFCReaderGen1.cpp ${ASSIMP_SOURCE_DIR}/IFCReaderGen2.cpp PROPERTIES COMPILE_FLAGS "-O2 -Wa,-mbig-obj")
   	endif()
   # endif (ASSIMP_BUILD_IFC_IMPORTER)

   target_include_directories(assimp PUBLIC ${ASSIMP_INCLUDE_DIR}) # For the SDK
   target_link_libraries(assimp PUBLIC utf8cpp clipper poly2tri open3dgc openddlparser irrXML rapidjson unzip minizip z)

   file(COPY ${ASSIMP_HEADERS} DESTINATION ${ASSIMP_INCLUDE_DIR}) # For the SDK
   file(COPY ${ASSIMP_HEADERS_II} DESTINATION ${ASSIMP_INCLUDE_DIR}/Compiler) # For the SDK

   # INCLUDE_DIRECTORIES(
       # ./
      # include
       # ${CMAKE_CURRENT_BINARY_DIR}
       # ${CMAKE_CURRENT_BINARY_DIR}/include
       # ${BENNI_SDK_INCLUDE_DIR}/include
   # )

   # Grouped compiler settings
#	IF ((CMAKE_C_COMPILER_ID MATCHES "GNU") AND NOT CMAKE_COMPILER_IS_MINGW)
#	  # hide all not-exported symbols
#	  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fvisibility=hidden -fPIC -Wall -std=c++0x")
#	  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
#	  set(LIBSTDC++_LIBRARIES -lstdc++)
#	ELSEIF(MSVC)
#	  # enable multi-core compilation with MSVC
#	  add_compile_options(/MP)
#
#	  # disable "elements of array '' will be default initialized" warning on MSVC2013
#	  IF(MSVC12)
#		 add_compile_options(/wd4351)
#	  ENDIF()
#	ELSEIF ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
#	  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fvisibility=hidden -fPIC -Wall -Wno-long-long -std=c++11" )
#	  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
#	ELSEIF( CMAKE_COMPILER_IS_MINGW )
#	  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -Wno-long-long -std=c++11" )
#	  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
#	  add_definitions( -U__STRICT_ANSI__ )
#	ENDIF()
#
#		# Compile internal irrXML only if system is not requested
#	if( NOT SYSTEM_IRRXML )
#		 add_subdirectory(contrib/irrXML)
#	endif( NOT SYSTEM_IRRXML )

   # if (ASSIMP_COVERALLS)
   #     MESSAGE(STATUS "Coveralls enabled")
   #     INCLUDE(Coveralls)
   #     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
   #     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
   # endif()

   # if (ASSIMP_WERROR)
   #   MESSAGE(STATUS "Treating warnings as errors")
   #   IF (MSVC)
   #     add_compile_options(/WX)
   #   ELSE()
   #     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
   #     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
   #   ENDIF()
   # endif()

   # if (ASSIMP_ASAN)
   #     MESSAGE(STATUS "AddressSanitizer enabled")
   #     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
   #     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
   # endif()

   # if (ASSIMP_UBSAN)
   #     MESSAGE(STATUS "Undefined Behavior sanitizer enabled")
   #     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fno-sanitize-recover=all")
   #     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -fno-sanitize-recover=all")
   # endif()

   # INCLUDE (FindPkgMacros)
   # INCLUDE (PrecompiledHeader)

   # If this is an in-source build (CMAKE_SOURCE_DIR == CMAKE_BINARY_DIR),
   # write the library/executable files to the respective directories in the
   # source tree. During an out-of-source build, however, do not litter this
   # directory, since that is probably what the user wanted to avoid.
   # IF ( CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR )
   #   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/lib" )
   #   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/lib" )
   #   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/bin" )
   # ENDIF ( CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR )

   # Cache these to allow the user to override them manually.
   # set(ASSIMP_LIB_INSTALL_DIR "lib" CACHE STRING "Path the built library files are installed to." )
   # set(ASSIMP_INCLUDE_INSTALL_DIR "include" CACHE STRING "Path the header files are installed to." )
   # set(ASSIMP_BIN_INSTALL_DIR "bin" CACHE STRING "Path the tool executables are installed to." )

   # IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
   #   set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Debug Postfix for lib, samples and tools")
   # ELSE()
   #   set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Debug Postfix for lib, samples and tools")
   # ENDIF()

   # Only generate this target if no higher-level project already has
   # IF (NOT TARGET uninstall)
   #   # add make uninstall capability
   #   CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
   #   add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
   # ENDIF()

   # cmake configuration files
   # CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/assimp-config.cmake.in"         "${CMAKE_CURRENT_BINARY_DIR}/assimp-config.cmake" @ONLY IMMEDIATE)
   # CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/assimp-config-version.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/assimp-config-version.cmake" @ONLY IMMEDIATE)
   # install(FILES "${CMAKE_CURRENT_BINARY_DIR}/assimp-config.cmake"             "${CMAKE_CURRENT_BINARY_DIR}/assimp-config-version.cmake" DESTINATION "${ASSIMP_LIB_INSTALL_DIR}/cmake/assimp-${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}" COMPONENT ${LIBASSIMP-DEV_COMPONENT})

   # FIND_PACKAGE( DirectX )

   # IF( BUILD_DOCS )
   #     add_subdirectory(doc)
   # ENDIF( BUILD_DOCS )

   # Look for system installed irrXML
   # IF ( SYSTEM_IRRXML )
   #     find_package( IrrXML REQUIRED )
   # ENDIF( SYSTEM_IRRXML )

   # Search for external dependencies, and build them from source if not found
   # Search for zlib
   # IF ( NOT ASSIMP_BUILD_ZLIB )
   #     find_package(ZLIB)
   # ENDIF( NOT ASSIMP_BUILD_ZLIB )

   # IF( NOT ZLIB_FOUND )
   #   message(STATUS "compiling zlib from souces")
   #   INCLUDE(CheckIncludeFile)
   #   INCLUDE(CheckTypeSize)
   #   INCLUDE(CheckFunctionExists)
   #   # compile from sources
   #   add_subdirectory(contrib/zlib)
   #   set(ZLIB_FOUND 1)
   #   set(ZLIB_LIBRARIES zlibstatic)
   #   set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/contrib/zlib ${CMAKE_CURRENT_BINARY_DIR}/contrib/zlib)
   # else(NOT ZLIB_FOUND)
   #   add_definitions(-DASSIMP_BUILD_NO_OWN_ZLIB)
   #   set(ZLIB_LIBRARIES_LINKED -lz)
   # ENDIF(NOT ZLIB_FOUND)
   # INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})

   # Search for unzip
   # use_pkgconfig(UNZIP minizip)

   # IF ( ASSIMP_NO_EXPORT )
   #   add_definitions( -DASSIMP_BUILD_NO_EXPORT)
   #   MESSAGE( STATUS "Build an import-only version of Assimp." )
   # ENDIF( ASSIMP_NO_EXPORT )

   # set(ASSIMP_BUILD_ARCHITECTURE "" CACHE STRING "describe the current architecture.")

   # IF    ( ASSIMP_BUILD_ARCHITECTURE STREQUAL "")
   # ELSE  ( ASSIMP_BUILD_ARCHITECTURE STREQUAL "")
   #   ADD_DEFINITIONS ( -D'ASSIMP_BUILD_ARCHITECTURE="${ASSIMP_BUILD_ARCHITECTURE}"' )
   # ENDIF ( ASSIMP_BUILD_ARCHITECTURE STREQUAL "")

   # ${CMAKE_GENERATOR}
   # set(ASSIMP_BUILD_COMPILER "" CACHE STRING "describe the current compiler.")
   # IF    ( ASSIMP_BUILD_COMPILER STREQUAL "")
   # ELSE  ( ASSIMP_BUILD_COMPILER STREQUAL "")
   #   ADD_DEFINITIONS ( -D'ASSIMP_BUILD_COMPILER="${ASSIMP_BUILD_COMPILER}"' )
   # ENDIF ( ASSIMP_BUILD_COMPILER STREQUAL "")

   # MARK_AS_ADVANCED( ASSIMP_BUILD_ARCHITECTURE ASSIMP_BUILD_COMPILER )


   # set(ASSIMP_BUILD_NONFREE_C4D_IMPORTER OFF CACHE BOOL "Build the C4D importer, relies on non-free Melange SDK.")
   # IF (ASSIMP_BUILD_NONFREE_C4D_IMPORTER)
   #   IF ( MSVC )
   #     set(C4D_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/contrib/Melange/includes")
   #
   #     # pick the correct prebuilt library
   #     IF(MSVC14)
   #       set(C4D_LIB_POSTFIX "_2015")
   #     ELSEIF(MSVC12)
   #       set(C4D_LIB_POSTFIX "_2013")
   #     ELSEIF(MSVC11)
   #       set(C4D_LIB_POSTFIX "_2012")
   #     ELSEIF(MSVC10)
   #       set(C4D_LIB_POSTFIX "_2010")
   #     ELSE()
   #       MESSAGE( FATAL_ERROR
   #         "C4D is currently only supported with MSVC 10, 11, 12, 14"
   #       )
   #     ENDIF()
   #
   #     set(C4D_LIB_BASE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/contrib/Melange/libraries/win")
   #
   #     set(C4D_DEBUG_LIBRARIES
   #       "${C4D_LIB_BASE_PATH}/melangelib${C4D_LIB_POSTFIX}/melangelib_debug.lib"
   #       "${C4D_LIB_BASE_PATH}/jpeglib${C4D_LIB_POSTFIX}/jpeglib_debug.lib"
   #     )
   #     set(C4D_RELEASE_LIBRARIES
   #       "${C4D_LIB_BASE_PATH}/melangelib${C4D_LIB_POSTFIX}/melangelib_release.lib"
   #       "${C4D_LIB_BASE_PATH}/jpeglib${C4D_LIB_POSTFIX}/jpeglib_release.lib"
   #     )
   #
   #     # winsock and winmm are necessary dependencies of melange (this is undocumented, but true.)
   #     set(C4D_EXTRA_LIBRARIES WSock32.lib Winmm.lib)
   #   ELSE ()
   #     MESSAGE( FATAL_ERROR
   #       "C4D is currently only available on Windows with melange SDK installed in contrib/Melange"
   #     )
   #   ENDIF ( MSVC )
   # ELSE (ASSIMP_BUILD_NONFREE_C4D_IMPORTER)
   #   add_definitions( -DASSIMP_BUILD_NO_C4D_IMPORTER )
   # ENDIF (ASSIMP_BUILD_NONFREE_C4D_IMPORTER)

   #####

   #####			add_subdirectory( src )

   #####


   # # Check dependencies for glTF importer with Open3DGC-compression.
   # # RT-extensions is used in "contrib/Open3DGC/o3dgcTimer.h" for collecting statistics. Pointed file
   # # has implementation for different platforms: WIN32, __MACH__ and other ("else" block).
   # FIND_PACKAGE(RT QUIET)
   # IF (RT_FOUND OR MSVC)
   # SET( ASSIMP_IMPORTER_GLTF_USE_OPEN3DGC 1 )
   # ADD_DEFINITIONS( -DASSIMP_IMPORTER_GLTF_USE_OPEN3DGC=1 )
   # ELSE ()
   # SET (open3dgc_SRCS "")
   # MESSAGE (INFO " RT-extension not found. glTF import/export will be built without Open3DGC-compression.")
   # #!TODO: off course is better to remove statistics timers from o3dgc codec. Or propose to choose what to use.
   # ENDIF ()

#	MESSAGE(STATUS "Enabled formats:${ASSIMP_IMPORTERS_ENABLED}")
#	MESSAGE(STATUS "Disabled formats:${ASSIMP_IMPORTERS_DISABLED}")


# 	if(ANDROID AND ASSIMP_ANDROID_JNIIOSYSTEM)
# 	set(ASSIMP_ANDROID_JNIIOSYSTEM_PATH port/AndroidJNI)
# 	add_subdirectory(../${ASSIMP_ANDROID_JNIIOSYSTEM_PATH}/ ../${ASSIMP_ANDROID_JNIIOSYSTEM_PATH}/)
# 	target_link_libraries(assimp android_jniiosystem)
# 	endif(ANDROID AND ASSIMP_ANDROID_JNIIOSYSTEM)

# 	IF (ASSIMP_BUILD_NONFREE_C4D_IMPORTER)
# 	TARGET_LINK_LIBRARIES(assimp optimized ${C4D_RELEASE_LIBRARIES})
# 	TARGET_LINK_LIBRARIES(assimp debug ${C4D_DEBUG_LIBRARIES})
# 	TARGET_LINK_LIBRARIES(assimp ${C4D_EXTRA_LIBRARIES})
# 	ENDIF (ASSIMP_BUILD_NONFREE_C4D_IMPORTER)

   # Add RT-extension library for glTF importer with Open3DGC-compression.
#	IF (RT_FOUND AND ASSIMP_IMPORTER_GLTF_USE_OPEN3DGC)
#	TARGET_LINK_LIBRARIES(assimp ${RT_LIBRARY})
#	ENDIF (RT_FOUND AND ASSIMP_IMPORTER_GLTF_USE_OPEN3DGC)

#	INSTALL( TARGETS assimp
#	LIBRARY DESTINATION ${ASSIMP_LIB_INSTALL_DIR}
#	ARCHIVE DESTINATION ${ASSIMP_LIB_INSTALL_DIR}
#	RUNTIME DESTINATION ${ASSIMP_BIN_INSTALL_DIR}
#	FRAMEWORK DESTINATION ${ASSIMP_LIB_INSTALL_DIR}
#	COMPONENT ${LIBASSIMP_COMPONENT})
#	INSTALL( FILES ${PUBLIC_HEADERS} DESTINATION ${ASSIMP_INCLUDE_INSTALL_DIR}/assimp COMPONENT assimp-dev)
#	INSTALL( FILES ${COMPILER_HEADERS} DESTINATION ${ASSIMP_INCLUDE_INSTALL_DIR}/assimp/Compiler COMPONENT assimp-dev)

#	if (ASSIMP_ANDROID_JNIIOSYSTEM)
#	INSTALL(FILES ${ASSIMP_HEADER_DIR}/${ASSIMP_ANDROID_JNIIOSYSTEM_PATH}/AndroidJNIIOSystem.h
#	DESTINATION ${ASSIMP_INCLUDE_INSTALL_DIR}
#	COMPONENT assimp-dev)
#	endif(ASSIMP_ANDROID_JNIIOSYSTEM)


   # IF ( ASSIMP_BUILD_ASSIMP_TOOLS )
   #   IF ( WIN32 AND DirectX_D3DX9_LIBRARY )
   #     option ( ASSIMP_BUILD_ASSIMP_VIEW "If the Assimp view tool is built. (requires DirectX)" ${DirectX_FOUND} )
   #     IF ( ASSIMP_BUILD_ASSIMP_VIEW )
   #       add_subdirectory( tools/assimp_view/ )
   #     ENDIF ( ASSIMP_BUILD_ASSIMP_VIEW )
   #   ENDIF ( WIN32 AND DirectX_D3DX9_LIBRARY )
   #
   #   add_subdirectory( tools/assimp_cmd/ )

     # Check dependencies for assimp_qt_viewer.
     # Why here? Maybe user do not want Qt viewer and have no Qt.
     # Why assimp_qt_viewer/CMakeLists.txt still contain similar check?
     # Because viewer can be build independently of Assimp.
   #   FIND_PACKAGE(Qt5Widgets QUIET)
   #   FIND_PACKAGE(DevIL QUIET)
   #   FIND_PACKAGE(OpenGL QUIET)
   #   IF ( Qt5Widgets_FOUND AND IL_FOUND AND OPENGL_FOUND)
   #     add_subdirectory( tools/assimp_qt_viewer/ )
   #   ELSE()
   #     set(ASSIMP_QT_VIEWER_DEPENDENCIES "")
   #     IF (NOT Qt5_FOUND)
   #       set(ASSIMP_QT_VIEWER_DEPENDENCIES "${ASSIMP_QT_VIEWER_DEPENDENCIES} Qt5")
   #     ENDIF (NOT Qt5_FOUND)
   #
   #     IF (NOT IL_FOUND)
   #       set(ASSIMP_QT_VIEWER_DEPENDENCIES "${ASSIMP_QT_VIEWER_DEPENDENCIES} DevIL")
   #     ENDIF (NOT IL_FOUND)
   #
   #     IF (NOT OPENGL_FOUND)
   #       set(ASSIMP_QT_VIEWER_DEPENDENCIES "${ASSIMP_QT_VIEWER_DEPENDENCIES} OpengGL")
   #     ENDIF (NOT OPENGL_FOUND)
   #
   #     MESSAGE (WARNING "Build of assimp_qt_viewer is disabled. Unsatisfied dendencies: ${ASSIMP_QT_VIEWER_DEPENDENCIES}")
   #   ENDIF ( Qt5Widgets_FOUND AND IL_FOUND AND OPENGL_FOUND)
   # ENDIF ( ASSIMP_BUILD_ASSIMP_TOOLS )

   # IF ( ASSIMP_BUILD_SAMPLES)
   #   IF ( WIN32 )
   #     add_subdirectory( samples/SimpleTexturedOpenGL/ )
   #   ENDIF ( WIN32 )
   #   add_subdirectory( samples/SimpleOpenGL/ )
   # ENDIF ( ASSIMP_BUILD_SAMPLES )

   # IF ( ASSIMP_BUILD_TESTS )
   #   add_subdirectory( test/ )
   # ENDIF ( ASSIMP_BUILD_TESTS )

   # Generate a pkg-config .pc for the Assimp library.
   # CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/assimp.pc.in" "${PROJECT_BINARY_DIR}/assimp.pc" @ONLY )
   # INSTALL( FILES "${PROJECT_BINARY_DIR}/assimp.pc" DESTINATION ${ASSIMP_LIB_INSTALL_DIR}/pkgconfig/ COMPONENT ${LIBASSIMP-DEV_COMPONENT})

   # IF(CMAKE_CPACK_COMMAND AND UNIX AND ASSIMP_OPT_BUILD_PACKAGES)
   #   # Packing information
   #   set(CPACK_PACKAGE_NAME                    "assimp{ASSIMP_VERSION_MAJOR}")
   #   set(CPACK_PACKAGE_CONTACT "" CACHE STRING "Package maintainer and PGP signer.")
   #   set(CPACK_PACKAGE_VENDOR                  "https://github.com/assimp")
   #   set(CPACK_PACKAGE_DISPLAY_NAME            "Assimp ${ASSIMP_VERSION}")
   #   set(CPACK_PACKAGE_DESCRIPTION_SUMMARY     " - Open Asset Import Library ${ASSIMP_VERSION}")
   #   set(CPACK_PACKAGE_VERSION                 "${ASSIMP_VERSION}.${ASSIMP_PACKAGE_VERSION}" )
   #   set(CPACK_PACKAGE_VERSION_MAJOR           "${ASSIMP_VERSION_MAJOR}")
   #   set(CPACK_PACKAGE_VERSION_MINOR           "${ASSIMP_VERSION_MINOR}")
   #   set(CPACK_PACKAGE_VERSION_PATCH           "${ASSIMP_VERSION_PATCH}")
   #   set(CPACK_PACKAGE_INSTALL_DIRECTORY       "assimp${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}")
   #   set(CPACK_RESOURCE_FILE_LICENSE           "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

   #   string(TOUPPER ${LIBASSIMP_COMPONENT}     "LIBASSIMP_COMPONENT_UPPER")
   #   string(TOUPPER ${LIBASSIMP-DEV_COMPONENT} "LIBASSIMP-DEV_COMPONENT_UPPER")

   #   set(CPACK_COMPONENT_ASSIMP-BIN_DISPLAY_NAME                       "tools")
   #   set(CPACK_COMPONENT_ASSIMP-BIN_DEPENDS                            "${LIBASSIMP_COMPONENT}" )
   #   set(CPACK_COMPONENT_${LIBASSIMP_COMPONENT_UPPER}_DISPLAY_NAME     "libraries")
   #   set(CPACK_COMPONENT_${LIBASSIMP-DEV_COMPONENT_UPPER}_DISPLAY_NAME "common headers and installs")
   #   set(CPACK_COMPONENT_${LIBASSIMP-DEV_COMPONENT_UPPER}_DEPENDS $    "{LIBASSIMP_COMPONENT}" )
   #   set(CPACK_COMPONENT_ASSIMP-DEV_DISPLAY_NAME                       "${CPACK_COMPONENT_${LIBASSIMP-DEV_COMPONENT}_DISPLAY_NAME}" )
   #   set(CPACK_COMPONENT_ASSIMP-DEV_DEPENDS                            "${LIBASSIMP-DEV_COMPONENT}" )
   #   set(CPACK_DEBIAN_BUILD_DEPENDS debhelper cmake zlib1g-dev pkg-config)

     # debian
   #   set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
   #   set(CPACK_DEBIAN_CMAKE_OPTIONS    "-DBUILD_ASSIMP_SAMPLES:BOOL=${ASSIMP_BUILD_SAMPLES}")
   #   set(CPACK_DEBIAN_PACKAGE_SECTION  "libs" )
   #   set(CPACK_DEBIAN_PACKAGE_DEPENDS  "${CPACK_COMPONENTS_ALL}")
   #   set(CPACK_DEBIAN_PACKAGE_SUGGESTS)
   #   set(CPACK_DEBIAN_PACKAGE_NAME     "assimp")
   #   set(CPACK_DEBIAN_PACKAGE_REMOVE_SOURCE_FILES contrib/cppunit-1.12.1 contrib/cppunit_note.txt contrib/zlib workspaces test doc obj samples packaging)
   #   set(CPACK_DEBIAN_PACKAGE_SOURCE_COPY svn export --force)
   #   set(CPACK_DEBIAN_CHANGELOG)
   #   execute_process(COMMAND lsb_release -is
   #     OUTPUT_VARIABLE _lsb_distribution OUTPUT_STRIP_TRAILING_WHITESPACE
   #     RESULT_VARIABLE _lsb_release_failed)
   #   set(CPACK_DEBIAN_DISTRIBUTION_NAME ${_lsb_distribution} CACHE STRING "Name of the distrubiton")
   #   STRING(TOLOWER ${CPACK_DEBIAN_DISTRIBUTION_NAME} CPACK_DEBIAN_DISTRIBUTION_NAME)
   #   IF( ${CPACK_DEBIAN_DISTRIBUTION_NAME} STREQUAL "ubuntu" )
   #     set(CPACK_DEBIAN_DISTRIBUTION_RELEASES lucid maverick natty oneiric precise CACHE STRING "Release code-names of the distrubiton release")
   #   ENDIF()
   #   set(DPUT_HOST "" CACHE STRING "PPA repository to upload the debian sources")
   #   include(CPack)
   #   include(DebSourcePPA)
   # ENDIF()

   # if(WIN32)
   #   if (CMAKE_SIZEOF_VOID_P EQUAL 8)
   #     set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin64/")
   #     set(LIB_DIR "${PROJECT_SOURCE_DIR}/lib64/")
   #   elseif()
   #     set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin32/")
   #     set(LIB_DIR "${PROJECT_SOURCE_DIR}/lib32/")
   #   ENDIF()
   #
   #   if(MSVC12)
   #     set(ASSIMP_MSVC_VERSION "vc120")
   #   elseif(MSVC14)
   #     set(ASSIMP_MSVC_VERSION "vc140")
   #   ENDIF(MSVC12)
   #
   #   if(MSVC12 OR MSVC14)
   #     add_custom_target(UpdateAssimpLibsDebugSymbolsAndDLLs COMMENT "Copying Assimp Libraries ..." VERBATIM)
   #     IF(CMAKE_GENERATOR MATCHES "^Visual Studio")
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/Release/assimp-${ASSIMP_MSVC_VERSION}-mt.dll	${BIN_DIR}assimp-${ASSIMP_MSVC_VERSION}-mt.dll VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/Release/assimp-${ASSIMP_MSVC_VERSION}-mt.exp	${LIB_DIR}assimp-${ASSIMP_MSVC_VERSION}-mt.exp VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/Release/assimp-${ASSIMP_MSVC_VERSION}-mt.lib	${LIB_DIR}assimp-${ASSIMP_MSVC_VERSION}-mt.lib VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/Debug/assimp-${ASSIMP_MSVC_VERSION}-mtd.dll		${BIN_DIR}assimp-${ASSIMP_MSVC_VERSION}-mtd.dll  VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/Debug/assimp-${ASSIMP_MSVC_VERSION}-mtd.exp		${LIB_DIR}assimp-${ASSIMP_MSVC_VERSION}-mtd.exp VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/Debug/assimp-${ASSIMP_MSVC_VERSION}-mtd.ilk		${LIB_DIR}assimp-${ASSIMP_MSVC_VERSION}-mtd.ilk VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/Debug/assimp-${ASSIMP_MSVC_VERSION}-mtd.lib		${LIB_DIR}assimp-${ASSIMP_MSVC_VERSION}-mtd.lib VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/Debug/assimp-${ASSIMP_MSVC_VERSION}-mtd.pdb		${LIB_DIR}assimp-${ASSIMP_MSVC_VERSION}-mtd.pdb VERBATIM)
   #     ELSE()
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/assimp-${ASSIMP_MSVC_VERSION}-mt.dll	${BIN_DIR}assimp-${ASSIMP_MSVC_VERSION}-mt.dll VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/assimp-${ASSIMP_MSVC_VERSION}-mt.exp	${LIB_DIR}assimp-${ASSIMP_MSVC_VERSION}-mt.exp VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/assimp-${ASSIMP_MSVC_VERSION}-mt.lib	${LIB_DIR}assimp-${ASSIMP_MSVC_VERSION}-mt.lib VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/assimp-${ASSIMP_MSVC_VERSION}-mtd.dll		${BIN_DIR}assimp-${ASSIMP_MSVC_VERSION}-mtd.dll  VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/assimp-${ASSIMP_MSVC_VERSION}-mtd.exp		${LIB_DIR}assimp-${ASSIMP_MSVC_VERSION}-mtd.exp VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/assimp-${ASSIMP_MSVC_VERSION}-mtd.ilk		${LIB_DIR}assimp-${ASSIMP_MSVC_VERSION}-mtd.ilk VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/assimp-${ASSIMP_MSVC_VERSION}-mtd.lib		${LIB_DIR}assimp-${ASSIMP_MSVC_VERSION}-mtd.lib VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/assimp-${ASSIMP_MSVC_VERSION}-mtd.pdb		${LIB_DIR}assimp-${ASSIMP_MSVC_VERSION}-mtd.pdb VERBATIM)
   #       ADD_CUSTOM_COMMAND(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/code/assimp-${ASSIMP_MSVC_VERSION}-mtd.pdb		${LIB_DIR}assimp-${ASSIMP_MSVC_VERSION}-mtd.pdb VERBATIM)
   #     ENDIF()
   #   ENDIF(MSVC12 OR MSVC14)
   # ENDIF (WIN32)





endif()

