#ifndef ABSTRACTMARKITEM_H
#define ABSTRACTMARKITEM_H
#include "globals.h"
//#include <QVariant>
//#include <QList>
#include <QAbstractGraphicsShapeItem>
#include <QPersistentModelIndex>
#include <QPen>
#include <QScopedPointer>
#include <functional>

//#include <QtContainerFwd>

//
//enum class SliceType;
//
//class AbstractMarkItem{
//	QString m_name;
//	double m_length;
//	QColor m_color;
//	SliceType m_type;
//	int m_sliceIndex;
//	bool m_visible;
//public:
//	AbstractMarkItem(const QString & name, double len, const QColor & c, SliceType type, int index, bool visible) :m_name(name), m_length(len), m_color(c), m_type(type), m_sliceIndex(index), m_visible(visible) {}
//	QString name()const noexcept { return m_name; }
//	double length() const noexcept { return m_length; }
//	QColor color()const noexcept { return m_color; }
//	SliceType sliceType()const noexcept { return m_type; }
//	void setSliceType(SliceType type)noexcept { m_type = type; }
//	bool checkState()const noexcept { return m_visible; }
//	void setCheckState(bool vis) noexcept { m_visible = vis; }
//	int sliceIndex()const noexcept { return m_sliceIndex; }
//	void setSliceIndex(int index) noexcept { m_sliceIndex = index; }
//	void setColor(const QColor & color) noexcept { m_color = color; }
//	void setName(const QString & name) noexcept { m_name = name; }
//protected:
//	inline void updateLength(double length) noexcept { m_length = length; }
//};
//
//Q_DECLARE_METATYPE(AbstractMarkItem*);
////Q_DECLARE_METATYPE(QGraphicsItem*);
//
//
//class StrokeMarkItem :public  QGraphicsItem, public AbstractMarkItem {
//	QRectF m_boundingRect;
//	QPainterPath m_painterPath;
//	QList<QPointF> m_points;
//public:
//	enum { Type = UserType + Mark };
//	StrokeMarkItem(QGraphicsItem * parent = nullptr, int index = -1, const QString & name = QString(), const QColor & color = Qt::black, SliceType type = SliceType::Top, bool visible = true);
//	QRectF boundingRect()const override { return m_boundingRect; }
//	QPainterPath shape()const override { return QPainterPath(); }
//	void addPoint(const QPointF & p);
//	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)override;
//	int type() const override { return Type; }
//private:
//	QRectF unionWith(const QRectF & rect, const QPointF & p);
//protected:
//	void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
//	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
//	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
//	void wheelEvent(QGraphicsSceneWheelEvent* event) Q_DECL_OVERRIDE;
//};
//
//
//
//class PolyMarkItem :public QGraphicsPolygonItem {
//public:
//	PolyMarkItem(QGraphicsItem * parent = nullptr, int index = -1, const QString & name = QString(), const QColor & color = Qt::black, SliceType type = SliceType::Top, bool visible = true) :QGraphicsPolygonItem(parent){}
//	PolyMarkItem(QPolygonF poly, QGraphicsItem * parent = nullptr, int index = -1, const QString & name = QString(), const QColor & color = Qt::black, SliceType type = SliceType::Top, bool visible = true) :QGraphicsPolygonItem(poly, parent){}
//};
//



//typedef QList<QPair<MarkProperty::Property, QString>> MarkPropertyInfo;



//Q_DECLARE_METATYPE(MarkPropertyInfo)




class StrokeMarkItem :public QGraphicsPolygonItem {
	std::function<QVariant(StrokeMarkItem* mark,QGraphicsItem::GraphicsItemChange, const QVariant &)> m_itemChangeHandler;
	QPersistentModelIndex m_modelIndex;

	struct StrokeMarkItemPrivate
	{
        StrokeMarkItemPrivate():m_visibleState(true),isFilled(false){}
		QString m_name;
		SliceType m_sliceType;
		double m_length;
		int m_index;
		bool m_visibleState;
        bool isFilled;

		friend QDataStream & operator<<(QDataStream & stream,const StrokeMarkItemPrivate * info) 
		{
			stream << info->m_name << static_cast<quint32>(info->m_sliceType) << info->m_length << info->m_index << info->m_visibleState;
			return stream;
		}

		friend QDataStream & operator>>(QDataStream & stream, StrokeMarkItemPrivate * info){
			Q_ASSERT(info);
			quint32 type;
			stream >> info->m_name >> type >> info->m_length >> info->m_index >> info->m_visibleState;
			info->m_sliceType = static_cast<SliceType>(type);
			return stream;
		}

	}*m_markInfo;

public:

	enum {Type = StrokeMark};
	explicit StrokeMarkItem(const QPolygonF& path, QGraphicsItem * parent=nullptr);
	explicit StrokeMarkItem(QGraphicsItem * parent = nullptr);
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) Q_DECL_OVERRIDE;
	void appendPoint(const QPointF & p);
	int type() const override { return Type; }
	void setItemChangeHandler(const std::function<QVariant(StrokeMarkItem* mark, QGraphicsItem::GraphicsItemChange, const QVariant&)>& handler);
	QPersistentModelIndex modelIndex()const { return m_modelIndex; }

	QString	name()const { return m_markInfo->m_name; }
	void setName(const QString & name) { m_markInfo->m_name = name; }
	SliceType sliceType()const { return m_markInfo->m_sliceType; }
	void setSliceType(SliceType type) { m_markInfo->m_sliceType = type; }
	double length()const { return m_markInfo->m_length; };
	int sliceIndex()const { return m_markInfo->m_index; }
	void setSliceIndex(int index) { m_markInfo->m_index = index; }

	bool visibleState()const { return m_markInfo->m_visibleState; }
	void setVisibleState(bool visible) { m_markInfo->m_visibleState = visible; }

    bool isFilled()const{return m_markInfo->isFilled;}
    void setFilled(bool fill){m_markInfo->isFilled = fill;update();}

	~StrokeMarkItem();

	friend QDataStream & operator<<(QDataStream & stream, const StrokeMarkItem * item);
	friend QDataStream & operator>>(QDataStream & stream, StrokeMarkItem *& item);
	
private:
	void createPropertyInfo();
	void updateLength();
	
protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
	friend class StrokeMarkTreeItem;		///< Member m_modelIndex
};






///**
// * \internal 
// * \brief This class represents a mark drawn by user on the slice.
// * 
// * The StrokeItem inherits from QGraphicsItem directly which is different from StrokeMarkItem
// * 
// */
//
//class NewStrokeMarkItemPrivate {
//public:
//
//};
//
//
//class NewStrokeMarkItem:public QGraphicsItem {
//public:
//	enum {Type = StrokeMark};
//	NewStrokeMarkItem(const QPolygonF & path, QGraphicsItem * parent = nullptr);
//	NewStrokeMarkItem(QGraphicsItem * parent = nullptr);
//	int type() const override { return Type; }
//	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
//	void appendPoint(const QPointF & p);
//private:
//	void createPropertInfo();
//	void updateLength();
//};

#endif // ABSTRACTMARKITEM_H
