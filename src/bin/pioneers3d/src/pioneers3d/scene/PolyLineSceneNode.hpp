#ifndef POLYLINERENDERER_HPP
#define POLYLINERENDERER_HPP

#if 0
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

class PolyLineRenderer : public QObject, protected QOpenGLFunctions
{
   Q_OBJECT

public:
   explicit PolyLineRenderer( QObject * parent = 0);
   virtual ~PolyLineRenderer();

   void render();

//    void rotateBy(int xAngle, int yAngle, int zAngle);
//    void setClearColor(const QColor &color);

//signals:
//    void updated();

//protected:


private:
   QOpenGLShaderProgram * m_Program;
   QOpenGLBuffer m_VertexBufferObject;
   QOpenGLTexture * textures[6];
   QVector<GLfloat> m_Vertices;
};

#endif
#endif // POLYLINERENDERER_HPP
