#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QMenu>

#include "renderwidget.h"
#include "abstract/abstractslicedatamodel.h"
#include "model/markmodel.h"
#include "3drender/shader/shaderprogram.h"
#include "3drender/geometry/slicevolume.h"
#include "algorithm/triangulate.h"
#include "globals.h"
#include "model/markitem.h"


const static int triIndex[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35 };

static QVector<QVector2D> cubeTex = {
	{ 0.f,0.f },{ 1.0f,0.f },{ 0.f,0.f },
{ 0.f,0.f },{ 0.f,1.0f },{ 0.f,0.f },
{ 0.f,0.f },{ 0.f,1.0f },{ 0.f,0.f },
{ 0.f,0.f },{ 0.f,1.0f },{ 0.f,0.f },
{ 0.f,0.f },{ 0.f,1.0f },{ 0.f,0.f },
{ 0.f,0.f },{ 0.f,1.0f },{ 0.f,0.f },
{ 0.f,0.f },{ 0.f,1.0f },{ 0.f,0.f },
{ 0.f,0.f },{ 0.f,1.0f },{ 0.f,0.f },
{ 0.f,0.f },{ 0.f,1.0f },{ 0.f,0.f },
{ 0.f,0.f },{ 0.f,1.0f },{ 0.f,0.f },
{ 0.f,0.f },{ 0.f,1.0f },{ 0.f,0.f },
{ 0.f,0.f },{ 0.f,1.0f },{ 0.f,0.f },
};

static QVector<QVector3D> cubeNor = {
	{ 0.f,0.f,-1.f },
{ 0.f,0.f,-1.f },
{ 0.f,0.f,-1.f },
{ 0.f,0.f,-1.f },
{ 0.f,0.f,-1.f },
{ 0.f,0.f,-1.f },
{ 0.f,0.f,1.f },
{ 0.f,0.f,1.f },
{ 0.f,0.f,1.f },
{ 0.f,0.f,1.f },
{ 0.f,0.f,1.f },
{ 0.f,0.f,1.f },
{ -1.0f,0.f,0.f },
{ -1.0f,0.f,0.f },
{ -1.0f,0.f,0.f },
{ -1.0f,0.f,0.f },
{ -1.0f,0.f,0.f },
{ -1.0f,0.f,0.f },
{ 1.0f,0.f,0.f },
{ 1.0f,0.f,0.f },
{ 1.0f,0.f,0.f },
{ 1.0f,0.f,0.f },
{ 1.0f,0.f,0.f },
{ 1.0f,0.f,0.f },
{ 0.f,-1.f,0.f },
{ 0.f,-1.f,0.f },
{ 0.f,-1.f,0.f },
{ 0.f,-1.f,0.f },
{ 0.f,-1.f,0.f },
{ 0.f,-1.f,0.f },
{ 0.f,1.f,0.f },
{ 0.f,1.f,0.f },
{ 0.f,1.f,0.f },
{ 0.f,1.f,0.f },
{ 0.f,1.f,0.f },
{ 0.f,1.f,0.f },
};


static QVector<QVector3D> cubeVertex =
{
	//back
	{ 0.5f, -0.5f, -0.5f },
{ -0.5f, -0.5f, -0.5f },
{ 0.5f,  0.5f, -0.5f },
{ -0.5f,  0.5f, -0.5f } ,
{ 0.5f,  0.5f, -0.5f } ,
{ -0.5f, -0.5f, -0.5f } ,
//front
{ -0.5f, -0.5f,  0.5f },
{ 0.5f, -0.5f,  0.5f },
{ 0.5f,  0.5f,  0.5f },
{ 0.5f,  0.5f,  0.5f },
{ -0.5f,  0.5f,  0.5f },
{ -0.5f, -0.5f,  0.5f },
//left
{ -0.5f,  0.5f,  0.5f },
{ -0.5f,  0.5f, -0.5f },
{ -0.5f, -0.5f, -0.5f },
{ -0.5f, -0.5f, -0.5f },
{ -0.5f, -0.5f,  0.5f },
{ -0.5f,  0.5f,  0.5f },
//right
{ 0.5f,  0.5f, -0.5f },
{ 0.5f,  0.5f,  0.5f },
{ 0.5f, -0.5f, -0.5f } ,
{ 0.5f, -0.5f,  0.5f } ,
{ 0.5f, -0.5f, -0.5f } ,
{ 0.5f,  0.5f,  0.5f } ,
//bottom
{ -0.5f, -0.5f, -0.5f },
{ 0.5f, -0.5f, -0.5f },
{ 0.5f, -0.5f,  0.5f },
{ 0.5f, -0.5f,  0.5f } ,
{ -0.5f, -0.5f,  0.5f } ,
{ -0.5f, -0.5f, -0.5f }  ,
//up
{ 0.5f,  0.5f, -0.5f }  ,
{ -0.5f,  0.5f, -0.5f }  ,
{ 0.5f,  0.5f,  0.5f }  ,
{ -0.5f,  0.5f,  0.5f } ,
{ 0.5f,  0.5f,  0.5f } ,
{ -0.5f,  0.5f, -0.5f }
};

static QVector<QVector3D> cubeVert =
{
	//back
	{ 0.5f, -0.5f, -0.5f },{ 0.f,0.f,-1.f },
{ -0.5f, -0.5f, -0.5f },{ 0.f,0.f,-1.f },
{ 0.5f,  0.5f, -0.5f },{ 0.f,0.f,-1.f },
{ -0.5f,  0.5f, -0.5f } ,{ 0.f,0.f,-1.f },
{ 0.5f,  0.5f, -0.5f } ,{ 0.f,0.f,-1.f },
{ -0.5f, -0.5f, -0.5f } ,{ 0.f,0.f,-1.f },
//front
{ -0.5f, -0.5f,  0.5f },{ 0.f,0.f,1.f },
{ 0.5f, -0.5f,  0.5f },{ 0.f,0.f,1.f },
{ 0.5f,  0.5f,  0.5f },{ 0.f,0.f,1.f },
{ 0.5f,  0.5f,  0.5f },{ 0.f,0.f,1.f },
{ -0.5f,  0.5f,  0.5f },{ 0.f,0.f,1.f },
{ -0.5f, -0.5f,  0.5f },{ 0.f,0.f,1.f },
//left
{ -0.5f,  0.5f,  0.5f },{ -1.0f,0.f,0.f },
{ -0.5f,  0.5f, -0.5f },{ -1.0f,0.f,0.f },
{ -0.5f, -0.5f, -0.5f },{ -1.0f,0.f,0.f },
{ -0.5f, -0.5f, -0.5f },{ -1.0f,0.f,0.f },
{ -0.5f, -0.5f,  0.5f },{ -1.0f,0.f,0.f },
{ -0.5f,  0.5f,  0.5f },{ -1.0f,0.f,0.f },
//right
{ 0.5f,  0.5f, -0.5f },{ 1.0f,0.f,0.f },
{ 0.5f,  0.5f,  0.5f },{ 1.0f,0.f,0.f },
{ 0.5f, -0.5f, -0.5f } ,{ 1.0f,0.f,0.f },
{ 0.5f, -0.5f,  0.5f } ,{ 1.0f,0.f,0.f },
{ 0.5f, -0.5f, -0.5f } ,{ 1.0f,0.f,0.f },
{ 0.5f,  0.5f,  0.5f } ,{ 1.0f,0.f,0.f },
//bottom
{ -0.5f, -0.5f, -0.5f },{ 0.f,-1.f,0.f },
{ 0.5f, -0.5f, -0.5f },{ 0.f,-1.f,0.f },
{ 0.5f, -0.5f,  0.5f },{ 0.f,-1.f,0.f },
{ 0.5f, -0.5f,  0.5f } ,{ 0.f,-1.f,0.f },
{ -0.5f, -0.5f,  0.5f } ,{ 0.f,-1.f,0.f },
{ -0.5f, -0.5f, -0.5f }  ,{ 0.f,-1.f,0.f },
//up
{ 0.5f,  0.5f, -0.5f }  ,{ 0.f,1.f,0.f },
{ -0.5f,  0.5f, -0.5f }  ,{ 0.f,1.f,0.f },
{ 0.5f,  0.5f,  0.5f }  ,{ 0.f,1.f,0.f },
{ -0.5f,  0.5f,  0.5f } ,{ 0.f,1.f,0.f },
{ 0.5f,  0.5f,  0.5f } ,{ 0.f,1.f,0.f },
{ -0.5f,  0.5f, -0.5f }  ,{ 0.f,1.f,0.f },
};

#define GLERROR(str)									\
	{													\
		GLenum err;										\
		while ((err = glGetError()) != GL_NO_ERROR)		\
		{												\
			std::cout<<err<<" "<<str<<std::endl;		\
		}												\
	}													\






/**
 * \brief Creates a widget for rendering
 * 
 * \note OpenGL 3.3 ProfileCore is needed.
 */
RenderWidget::RenderWidget(AbstractSliceDataModel * dataModel,
	MarkModel * markModel,

	QWidget * parent)
	:QOpenGLWidget(parent),
	m_markModel(markModel),
	m_dataModel(dataModel),
	//m_parameterWidget(widget),
	m_camera(QVector3D(0.f, 0.f, 10.f)),
	m_rayStep(0.02),
	m_tfTexture(nullptr),
	m_volume(nullptr),
	m_meshShader(nullptr),
	m_selectShader(nullptr),
	m_pickFBO(nullptr),
	d_ptr(new RenderWidgetPrivate(this))
{
	//m_contextMenu = new QMenu(QStringLiteral("Context Menu"), this);

	//Q_ASSERT_X(widget != nullptr, "VolumeWidget::VolumeWidget", "null pointer");
	//connect(widget, &RenderParameterWidget::optionsChanged, [this]() {update(); });
	//connect(widget, &RenderParameterWidget::markUpdated,this,&RenderWidget::updateMark);
}

/**
 * \brief Sets the data model as the given \a model
 * 
 * \note This function will emit requireTransferFunction() signal and dataModelChanged() signal
 */
void RenderWidget::setDataModel(AbstractSliceDataModel * model)
{
	m_dataModel = model;
	updateVolumeData();
	emit requireTransferFunction();
	emit dataModelChanged();
	update();
}

/**
 * \brief Sets the slice model as the given \a model
 * 
 * \note This function will emit markModelChanged() signal
 */
void RenderWidget::setMarkModel(MarkModel* model)
{
	if(m_markModel != nullptr && m_markModel != model) {
		disconnect(m_markModel, &MarkModel::dataChanged, this, &RenderWidget::_slot_markModelDataChanged);
		disconnect(m_markModel->selectionModelOfThisModel(), &QItemSelectionModel::currentChanged, this, &RenderWidget::_slot_currentChanged_selectionModel);
		disconnect(m_markModel->selectionModelOfThisModel(), &QItemSelectionModel::selectionChanged, this, &RenderWidget::_slot_selectionChanged_selectionModel);
	}

	m_markModel = model;
	connect(m_markModel, &MarkModel::dataChanged, this, &RenderWidget::_slot_markModelDataChanged);
	connect(m_markModel->selectionModelOfThisModel(), &QItemSelectionModel::currentChanged, this, &RenderWidget::_slot_currentChanged_selectionModel);
	connect(m_markModel->selectionModelOfThisModel(), &QItemSelectionModel::selectionChanged, this, &RenderWidget::_slot_selectionChanged_selectionModel);

	updateMark();
	//emit markModelChanged();
	emit markModelChanged();

	update();		// Repaint immediately
}

//ShaderDataInterface

/**
 * \brief Returns the volume object
 * 
 * \sa GPUVolume
 */
GPUVolume* RenderWidget::volume() const
{
	return m_volume.data();
}

/**
 * \brief Reimplemented from QOpenGLWidget::minimumSizeHint()
 */
QSize RenderWidget::minimumSizeHint() const
{
	return QSize(400, 300);
}

/**
 * \brief Reimplemented from QOpenGLWidget::sizeHint()
 */
QSize RenderWidget::sizeHint() const
{
	return QSize(800, 600);
}

/**
 * \brief Reimplemented from QOpenGLWidget::initializeGL()
 * 
 * Initializes OpenGL related resources.
 */
void RenderWidget::initializeGL()
{
	if (!initializeOpenGLFunctions()) {
		//qFatal("initializeOpenGLFunctions failed");
		return;
	}
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &RenderWidget::cleanup);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	// Update transfer functions
	emit requireTransferFunction();

	makeCurrent();
	if (m_volume != nullptr)
		m_volume->initializeGLResources();

	for (auto & item : m_integration)
		item.mesh->initializeGLResources();

	// mesh shader

	m_meshShader = new QOpenGLShaderProgram;
	m_meshShader->create();
    m_meshShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/mesh_shader_v.glsl");
    m_meshShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/mesh_shader_f.glsl");
	if (m_meshShader->link() == false) {
		qFatal("Mesh shader is not linked.");
		return;
	}
	// pick shader
	m_selectShader = new QOpenGLShaderProgram;
	m_selectShader->create();
    m_selectShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/pick_shader_v.glsl");
    m_selectShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/pick_shader_f.glsl");
	if (m_selectShader->link() == false) {
		qFatal("Select shader is not linked.");
		return;
	}
	doneCurrent();
}

//#define EXPORT_FBO_IMG

/**
 * \brief Reimplemented from QOpenGLWidget::resizeGL()
 * 
 * \note This function will emit windowResize(int w,int h) signal
 */
void RenderWidget::resizeGL(int w, int h)
{
	// Update projection matrices
	const double aspect = GLfloat(w) / h;
	m_proj.setToIdentity();
	m_otho.setToIdentity();
	m_otho.ortho(-aspect * 2, aspect * 2, -2, 2, -100.0, 100.0);
	m_proj.perspective(45.f, aspect, 0.01f, 100.f);

	if (m_pickFBO != nullptr);
	delete m_pickFBO;
	m_pickFBO = new QOpenGLFramebufferObject(w, h, QOpenGLFramebufferObject::Depth, GL_TEXTURE_RECTANGLE, GL_RGBA32F_ARB);

	emit windowResized(w, h);
}

/**
 * \brief Reimplemented from QOpenGLWidget::paintGL()
 * 
 * Rendering code here
 */
void RenderWidget::paintGL()
{
	//Q_ASSERT_X(m_parameterWidget != nullptr, "VolumeWidget::paintGL", "null pointer");
	Q_D(RenderWidget);

	const auto renderMode = d->options->mode;
	//
	const auto xs = d->options->xSpacing;
	const auto ys = d->options->ySpacing;
	const auto zs = d->options->zSpacing;

	QMatrix4x4 world;
	world.setToIdentity();
	world.scale(xs, ys, zs);

	//
	m_camera.setCenter(d->volumeNormalTransform*world*QVector3D(0.5, 0.5, 0.5));

	if (m_volume != nullptr) {
		if (renderMode == RenderMode::DVR)
			m_volume->sliceMode(false);
		else {
			m_volume->sliceMode(true);
			m_volume->setSliceSphereCoord(d->options->sliceNormal);
		}

		m_volume->setTransform(world);
		m_volume->render();
	}

	if (renderMode != RenderMode::DVR) {

		const auto viewMatrix = camera().view();
		const auto cameraPos = camera().position();


		if (d->enableStartPicking == true) {
			m_pickFBO->bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_selectShader->bind();
			m_selectShader->setUniformValue("viewMatrix", viewMatrix);
			m_selectShader->setUniformValue("projMatrix", m_proj);
			m_selectShader->setUniformValue("modelMatrix", world);

			for (int i = 0; i < m_integration.size(); i++) {
				if(m_integration[i].visible == true) {
					const auto color = idToColor(i);
					m_selectShader->setUniformValue("pickColor", color);
					m_integration[i].mesh->render();
				}
			}
			m_selectShader->release();
			m_pickFBO->release();
		}

		m_meshShader->bind();
		m_meshShader->setUniformValue("viewMatrix", viewMatrix);
		m_meshShader->setUniformValue("projMatrix", m_proj);
		m_meshShader->setUniformValue("modelMatrix", world);
		m_meshShader->setUniformValue("normalMatrix", world.normalMatrix());
		m_meshShader->setUniformValue("lightPos", cameraPos);
		m_meshShader->setUniformValue("viewPos", cameraPos);

		for (int i = 0; i < m_integration.size(); i++) {

			if (m_integration[i].visible == false)
				continue;

			QColor color = m_integration[i].color;

			if (i == d->selectedObjectId) {
				//glClear(GL_STENCIL_BUFFER_BIT);
				//glEnable(GL_STENCIL_TEST);
				//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

				//glStencilFunc(GL_ALWAYS, 1, 0xFF);
				//glStencilMask(0xFF);
				color = m_integration[i].color.lighter(150);
				//m_meshShader->setUniformValue("objectColor", color);
				//m_markMeshes[i]->render();

				//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				//glStencilMask(0x00);
				//glDisable(GL_DEPTH_TEST);
				//m_meshShader->setUniformValue("outlining", true);
				m_meshShader->setUniformValue("outlining", false);
				m_meshShader->setUniformValue("objectColor", color);
				m_meshShader->setUniformValue("modelMatrix", world);
				m_integration[i].mesh->render();
				//glStencilMask(0xFF);
				//glEnable(GL_DEPTH_TEST);
				//glDisable(GL_STENCIL_TEST);
			}
			else {
				m_meshShader->setUniformValue("outlining", false);
				m_meshShader->setUniformValue("objectColor", color);
				m_meshShader->setUniformValue("modelMatrix", world);
				m_integration[i].mesh->render();
			}


		}

		m_meshShader->release();
	}
}

void RenderWidget::mousePressEvent(QMouseEvent* event)
{
	Q_D(RenderWidget);
	d->lastMousePos = event->pos();
}

void RenderWidget::mouseMoveEvent(QMouseEvent* event)
{
	Q_D(RenderWidget);
	const auto & p = event->pos();
	// Update Camera
	float dx = p.x() - d->lastMousePos.x();
	float dy = d->lastMousePos.y() - p.y();
	if ((event->buttons() & Qt::LeftButton) && (event->buttons() & Qt::RightButton))
	{
		const auto direction = m_camera.up()*dy + m_camera.right()*dx;
		m_camera.movement(direction, 0.002);
	}
	else if (event->buttons() & Qt::LeftButton)
	{
		m_camera.rotation(dx, dy, QVector3D(0, 0, 0));
	}
	else if (event->buttons() == Qt::RightButton)
	{
		const auto direction = m_camera.front()*dy;
		m_camera.movement(direction, 0.01);
	}
	d->lastMousePos = p;
	//
	update();
}

/**
 * \brief Reimplemented from QOpenGLWidget::mouseReleaseEvent(QMouseEvent * event)
 *
 * This event handler is used to implement the mouse picking feature.
 * \param event
 */
void RenderWidget::mouseReleaseEvent(QMouseEvent* event) {
	Q_D(RenderWidget);
	if (d->enablePickingMode == true) {
		d->enableStartPicking = true;
		repaint();			// Paint the object into the color frame buffer immediately.
		const auto & p = event->pos();
		const auto previous = d->selectedObjectId;
		d->selectedObjectId = selectMesh(p.x(), p.y());

	
		_slot_currentMeshChanged(d->selectedObjectId, previous);
		d->enableStartPicking = false;
		repaint();			// Show the result immediately
	}
	update();
}

/**
 * \brief Updates the transfer function that volume rendering uses
 */
void RenderWidget::updateTransferFunction(const float * func)
{
	makeCurrent();
	if (m_tfTexture == nullptr)
	{
		//m_tfTexture.isCreated() == false
		m_tfTexture = new QOpenGLTexture(QOpenGLTexture::Target1D);
		m_tfTexture->setMagnificationFilter(QOpenGLTexture::Linear);
		m_tfTexture->setMinificationFilter(QOpenGLTexture::Linear);
		m_tfTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
		m_tfTexture->setSize(256);
		m_tfTexture->setFormat(QOpenGLTexture::RGBA16_UNorm);		//Equivalent to GL_RGBA16
		m_tfTexture->allocateStorage();
	}
	m_tfTexture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::Float32, func);	//Equivalent to GL_RGBA and GL_FLOAT
	doneCurrent();
	update();
}



/**
 * \brief This property holds the visibility of the top slice when renders data with slice type render.
 */
void RenderWidget::setTopSliceVisible(bool check)
{
	if (m_volume != nullptr) {
		m_volume->setTopSliceVisible(check);
		update();
	}
}

/**
 * \brief This property holds the visibility of the right slice when renders data with slice type render.
 */
void RenderWidget::setRightSliceVisible(bool check) {

	if (m_volume != nullptr) {
		m_volume->setRightSliceVisible(check);
		update();
	}
}

/**
 * \brief This property holds the visibility of the front slice when renders data with slice type render.
 */
void RenderWidget::setFrontSliceVisible(bool check) {

	if (m_volume != nullptr) {
		m_volume->setFrontSliceVisible(check);
		update();
	}
}

/**
 * \brief This is a slot function
 */
void RenderWidget::_slot_markModelDataChanged(const QModelIndex & begin, const QModelIndex & end, const QVector<int>& role)
{
	if (begin != end || begin.isValid() ==false)
		return;
	if(role[0] == Qt::CheckStateRole) {
		const auto id = m_query.query(begin);
		if(id != -1) {
			const auto item = static_cast<InstanceTreeItem*>(begin.internalPointer());
			m_integration[id].visible = item->visible();
			update();
		}
	}

}

/**
 * \brief This is a private slot function
 */
void RenderWidget::_slot_currentMeshChanged(int current, int previous)
{
	//qDebug() << "RenderWidget::_slot_currentMeshChanged " << " RenderWidget should be clicked";
	if(m_markModel == nullptr) 
		return;
	const auto index = m_query.query(current);
	//m_markModel->selectionModelOfThisModel()->clear();
	m_markModel->selectionModelOfThisModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);

}

/**
 * \brief This is a private slot function
 */
void RenderWidget::_slot_currentChanged_selectionModel(const QModelIndex& current,
	const QModelIndex& previous) {

	const auto treeItem = static_cast<TreeItem*>(current.internalPointer());
	if(treeItem != nullptr) {
		if (treeItem->type() == TreeItemType::Mark) {

			// The parent item of a mark item, which should be a mesh item should also be selected

			//qDebug() << "RenderWidget::_slot_currentChanged_selectionModel " << " Mark should be clicked from SliceEditorWidget";
			const auto parent = m_markModel->parent(current);
			if (static_cast<TreeItem*>(parent.internalPointer())->type() == TreeItemType::Instance) {
				const auto id = m_query.query(parent);
				if (id != -1) {
					Q_D(RenderWidget);
					d->selectedObjectId = id;
					update();
				}
			}

		}
		else {		// Mesh item is selected
			//qDebug() << "RenderWidget::_slot_currentChanged_selectionModel " << " Instance should be clicked from QTreeView";
			const auto id = m_query.query(current);
			if (id != -1) {
				Q_D(RenderWidget);
				d->selectedObjectId = id;
				update();
			}
		}
	}

}

/**
 * \brief This is a private slot function
 */
void RenderWidget::_slot_selectionChanged_selectionModel(const QItemSelection & selected, const QItemSelection & deselected)
{

}

/**
 * \brief This is a private 
 */
void RenderWidget::updateMark() {

	Q_D(RenderWidget);
	if (m_markModel == nullptr || m_dataModel == nullptr)
	{
		return;
	}
	
	m_integration.clear();
	m_query.clear();
	//const auto cates = m_markModel->categoryText();
	makeCurrent();

	const auto z = m_dataModel->topSliceCount();
	const auto y = m_dataModel->rightSliceCount();
	const auto x = m_dataModel->frontSliceCount();

	QMatrix4x4 trans;
	trans.setToIdentity();
	trans.scale(1 / static_cast<double>(x), 1 / static_cast<double>(y), 1 / static_cast<double>(z));
	{
		const auto instances = m_markModel->treeItems(QModelIndex(), TreeItemType::Instance);
		const QColor color = Qt::red;
		for (const auto & inst : instances) {
			const auto mesh = static_cast<InstanceTreeItem*>(inst)->mesh();
			Q_ASSERT_X(mesh->isReady(), "RenderWidget::updateMark", "Mesh not ready");
			const auto v = mesh->vertices();
			const auto idx = mesh->indices();
			const auto nV = mesh->vertexCount();
			const auto nT = mesh->triangleCount();
			const auto n = mesh->normals();
			auto ptr = QSharedPointer<TriangleMesh>(new TriangleMesh(v,
				n,
				nullptr,
				nV,
				idx,
				nT,
				d->volumeNormalTransform*trans,		//Make mesh coordinate matching with normalized volume coordinates
				this));
			ptr->initializeGLResources();
			m_integration.push_back({ptr,color,true});

			const auto id = m_integration.size() - 1;
			m_query.addQueryPair(inst->persistentModelIndex(), id);
		}
	}
	doneCurrent();
}

/**
 * \brief 
 */
void RenderWidget::updateVolumeData()
{
	if (m_dataModel == nullptr)
		return;
	const auto z = m_dataModel->topSliceCount();
	const auto y = m_dataModel->rightSliceCount();
	const auto x = m_dataModel->frontSliceCount();

	Q_D(RenderWidget);

	QVector3D m_scale = QVector3D(x, y, z);
	m_scale.normalize();

	d->volumeNormalTransform.setToIdentity();		//Transform size from (1,1,1) to (x/N,y/N,z/N)
	d->volumeNormalTransform.scale(m_scale);
	QMatrix4x4 I;
	I.setToIdentity();

	// Decide data format

	VolumeFormat fmt;
	fmt.fmt = VoxelFormat::Grayscale;			// AbstractSliceDataModel only support 1-channel data
	if(m_dataModel->dataType() == 0)		// uint8
	{
		fmt.type = VoxelType::UInt8;
	}else if(m_dataModel->dataType() == 1) {
		fmt.type = VoxelType::Float32;
	}

	m_volume.reset(new SliceVolume(m_dataModel->constRawData(),x,y,z,I, fmt, this));
	makeCurrent();
	m_volume->initializeGLResources();
	doneCurrent();
}


int RenderWidget::selectMesh(int x, int y)
{
	makeCurrent();
	m_pickFBO->bind();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	unsigned char color[4];
	glReadPixels(x, size().height() - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	const auto id = colorToId(QColor(color[0], color[1], color[2]));
	m_pickFBO->release();
	doneCurrent();
	return id;
}


void RenderWidget::cleanup()
{
	makeCurrent();

	if (m_volume != nullptr)
		m_volume->destroyGLResources();

	for (auto & inte : m_integration)
		inte.mesh->destoryGLResources();

	delete m_meshShader;
	m_meshShader = nullptr;
	delete m_selectShader;
	m_selectShader = nullptr;
	delete m_tfTexture;
	m_tfTexture = nullptr;
	delete m_pickFBO;
	m_pickFBO = nullptr;

	doneCurrent();
}

/**
 * \brief Destroyes the render widget.
 * 
 * \note disconnect the QOpenGLContext::aboutToBeDestroyed() signal from QOpenGLContext links to \a RenderWidget::cleanup() is necessary
 * Because the context is not destroyed yet after the widget is destroyed. The signal still emits and will activate a empty slot and the 
 * program will crash. 
 * 
 * \sa QOpenGLContext RenderWidget:cleanup()
 */
RenderWidget::~RenderWidget()
{
	cleanup();			//
	Q_D(RenderWidget);
	delete d;

	/*!
		Critical: The context will emit aboutToBeDestroyed() signal and will cause a crash
		even after this class was destroyed if below disconnection had not been called
	 */

	disconnect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &RenderWidget::cleanup);
}
