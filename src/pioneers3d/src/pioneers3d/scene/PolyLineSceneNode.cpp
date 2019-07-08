#include "PolyLineSceneNode.hpp"

#if 0
QMatrix4x4 computeCamera()
{
    QMatrix4x4 u_mvp;
    u_mvp.ortho(-0.5f, +0.5f, +0.5f, -0.5f, 4.0f, 15.0f);
    u_mvp.translate(0.0f, 0.0f, -10.0f);
    u_mvp.rotate(20.0 / 16.0f, 1.0f, 0.0f, 0.0f);
    u_mvp.rotate(30.0 / 16.0f, 0.0f, 1.0f, 0.0f);
    u_mvp.rotate( 0.0 / 16.0f, 0.0f, 0.0f, 1.0f);
    return u_mvp;
}

PolyLineRenderer::PolyLineRenderer( QObject * parent )
    : QObject( parent )
    , m_Program(0)
{
    initializeOpenGLFunctions();

    // makeObject();

    static const int coords[6][4][3] = {
        { { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 } },
        { { +1, +1, -1 }, { -1, +1, -1 }, { -1, +1, +1 }, { +1, +1, +1 } },
        { { +1, -1, +1 }, { +1, -1, -1 }, { +1, +1, -1 }, { +1, +1, +1 } },
        { { -1, -1, -1 }, { -1, -1, +1 }, { -1, +1, +1 }, { -1, +1, -1 } },
        { { +1, -1, +1 }, { -1, -1, +1 }, { -1, -1, -1 }, { +1, -1, -1 } },
        { { -1, -1, +1 }, { +1, -1, +1 }, { +1, +1, +1 }, { -1, +1, +1 } }
    };

    #ifdef USE_TEXTURE_STAGE_0
    memset(textures, 0, sizeof(textures));

    for (int j = 0; j < 6; ++j)
        textures[j] = new QOpenGLTexture(QImage(QString(":/images/side%1.png").arg(j + 1)).mirrored());
    #endif

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            // vertex position
            m_Vertices.append(0.2 * coords[i][j][0]);
            m_Vertices.append(0.2 * coords[i][j][1]);
            m_Vertices.append(0.2 * coords[i][j][2]);
            // texture coordinate
            m_Vertices.append(j == 0 || j == 3);
            m_Vertices.append(j == 0 || j == 1);
        }
    }

    m_VertexBufferObject.create();
    m_VertexBufferObject.bind();
    m_VertexBufferObject.allocate(  m_Vertices.constData(),
                                    m_Vertices.count() * sizeof(GLfloat) );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    #define SHADER_PROGRAM_A_POS 0
    #define SHADER_PROGRAM_A_TEX0 1

    QOpenGLShader * vshader = new QOpenGLShader( QOpenGLShader::Vertex, this );
    const char * vsrc =
        "attribute 	highp 	vec4 a_pos;\n"
       // "attribute 	mediump vec4 a_tex;\n"
       // "varying 	mediump vec4 v_tex;\n"
        "uniform 	mediump mat4 u_mvp;\n"
        "void main(void)\n"
        "{\n"
       // "    v_tex = a_tex;		\n"
        "    gl_Position = u_mvp * a_pos;	\n"
        "}\n";
    vshader->compileSourceCode(vsrc);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
#ifdef USE_TEXTURE_STAGE_0
    const char *fsrc =
        "varying mediump vec4 	   v_tex;   \n"
        "uniform 		 sampler2D u_stage0; \n"
        "void main(void)			     	  \n"
        "{\n"
        "    gl_FragColor = texture2D( u_stage0, v_tex.st );\n"
        "}\n";
#else
    const char *fsrc =
        "void main(void)			     	  \n"
        "{\n"
        "    gl_FragColor = vec4( 1.0f, 0.8f, 0.3f, 1.0f );\n"
        "}\n";
#endif
    fshader->compileSourceCode(fsrc);

    m_Program = new QOpenGLShaderProgram;
    m_Program->addShader(vshader);
    m_Program->addShader(fshader);
    m_Program->bindAttributeLocation("a_pos", SHADER_PROGRAM_A_POS );
    //m_Program->bindAttributeLocation("a_tex", SHADER_PROGRAM_A_TEX0 );
    m_Program->link();
    m_Program->bind();
    //m_Program->setUniformValue( "u_stage0", 0 );
    m_Program->setUniformValue( "u_mvp", computeCamera() );
}

PolyLineRenderer::~PolyLineRenderer()
{
    //makeCurrent();
    m_VertexBufferObject.destroy();
    //for (int i = 0; i < 6; ++i)
    //    delete textures[i];
    delete m_Program;
    //doneCurrent();
}

void PolyLineRenderer::render()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //m_Program->setUniformValue( "u_stage0", 0 );
    m_Program->setUniformValue( "u_mvp", computeCamera() );
    m_Program->enableAttributeArray( SHADER_PROGRAM_A_POS);
    //m_Program->enableAttributeArray( SHADER_PROGRAM_A_TEX0);
    m_Program->setAttributeBuffer( SHADER_PROGRAM_A_POS, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    //m_Program->setAttributeBuffer( SHADER_PROGRAM_A_TEX0, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

    for (int i = 0; i < 6; ++i)
    {
        //textures[i]->bind();
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }
}

#endif
