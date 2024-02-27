/****************************************************************************
** Meta object code from reading C++ file 'slicewidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../src/tomomain/ImageViewWindow/widgets/slicewidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'slicewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SliceWidget_t {
    QByteArrayData data[27];
    char stringdata0[306];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SliceWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SliceWidget_t qt_meta_stringdata_SliceWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SliceWidget"
QT_MOC_LITERAL(1, 12, 13), // "sliceSelected"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 5), // "point"
QT_MOC_LITERAL(4, 33, 9), // "markAdded"
QT_MOC_LITERAL(5, 43, 15), // "StrokeMarkItem*"
QT_MOC_LITERAL(6, 59, 4), // "item"
QT_MOC_LITERAL(7, 64, 9), // "viewMoved"
QT_MOC_LITERAL(8, 74, 5), // "delta"
QT_MOC_LITERAL(9, 80, 16), // "selectionChanged"
QT_MOC_LITERAL(10, 97, 12), // "setOperation"
QT_MOC_LITERAL(11, 110, 5), // "state"
QT_MOC_LITERAL(12, 116, 8), // "setImage"
QT_MOC_LITERAL(13, 125, 5), // "image"
QT_MOC_LITERAL(14, 131, 6), // "setPen"
QT_MOC_LITERAL(15, 138, 3), // "pen"
QT_MOC_LITERAL(16, 142, 14), // "setDefaultZoom"
QT_MOC_LITERAL(17, 157, 24), // "setNavigationViewEnabled"
QT_MOC_LITERAL(18, 182, 7), // "enabled"
QT_MOC_LITERAL(19, 190, 21), // "navigationViewEnabled"
QT_MOC_LITERAL(20, 212, 15), // "clearSliceMarks"
QT_MOC_LITERAL(21, 228, 13), // "selectedItems"
QT_MOC_LITERAL(22, 242, 22), // "QList<StrokeMarkItem*>"
QT_MOC_LITERAL(23, 265, 17), // "selectedItemCount"
QT_MOC_LITERAL(24, 283, 9), // "moveSlice"
QT_MOC_LITERAL(25, 293, 3), // "dir"
QT_MOC_LITERAL(26, 297, 8) // "sizeHint"

    },
    "SliceWidget\0sliceSelected\0\0point\0"
    "markAdded\0StrokeMarkItem*\0item\0viewMoved\0"
    "delta\0selectionChanged\0setOperation\0"
    "state\0setImage\0image\0setPen\0pen\0"
    "setDefaultZoom\0setNavigationViewEnabled\0"
    "enabled\0navigationViewEnabled\0"
    "clearSliceMarks\0selectedItems\0"
    "QList<StrokeMarkItem*>\0selectedItemCount\0"
    "moveSlice\0dir\0sizeHint"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SliceWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06 /* Public */,
       1,    0,  102,    2, 0x06 /* Public */,
       4,    1,  103,    2, 0x06 /* Public */,
       7,    1,  106,    2, 0x06 /* Public */,
       9,    0,  109,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,  110,    2, 0x0a /* Public */,
      12,    1,  113,    2, 0x0a /* Public */,
      14,    1,  116,    2, 0x0a /* Public */,
      16,    0,  119,    2, 0x0a /* Public */,
      17,    1,  120,    2, 0x0a /* Public */,
      19,    0,  123,    2, 0x0a /* Public */,
      15,    0,  124,    2, 0x0a /* Public */,
      20,    0,  125,    2, 0x0a /* Public */,
      21,    0,  126,    2, 0x0a /* Public */,
      23,    0,  127,    2, 0x0a /* Public */,
      24,    1,  128,    2, 0x0a /* Public */,
      26,    0,  131,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPoint,    3,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QPointF,    8,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::QImage,   13,
    QMetaType::Void, QMetaType::QPen,   15,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Bool,
    QMetaType::QPen,
    QMetaType::Void,
    0x80000000 | 22,
    QMetaType::Int,
    QMetaType::Void, QMetaType::QPointF,   25,
    QMetaType::QSize,

       0        // eod
};

void SliceWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SliceWidget *_t = static_cast<SliceWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sliceSelected((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 1: _t->sliceSelected(); break;
        case 2: _t->markAdded((*reinterpret_cast< StrokeMarkItem*(*)>(_a[1]))); break;
        case 3: _t->viewMoved((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 4: _t->selectionChanged(); break;
        case 5: _t->setOperation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setImage((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        case 7: _t->setPen((*reinterpret_cast< const QPen(*)>(_a[1]))); break;
        case 8: _t->setDefaultZoom(); break;
        case 9: _t->setNavigationViewEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: { bool _r = _t->navigationViewEnabled();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { QPen _r = _t->pen();
            if (_a[0]) *reinterpret_cast< QPen*>(_a[0]) = _r; }  break;
        case 12: _t->clearSliceMarks(); break;
        case 13: { QList<StrokeMarkItem*> _r = _t->selectedItems();
            if (_a[0]) *reinterpret_cast< QList<StrokeMarkItem*>*>(_a[0]) = _r; }  break;
        case 14: { int _r = _t->selectedItemCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 15: _t->moveSlice((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 16: { QSize _r = _t->sizeHint();
            if (_a[0]) *reinterpret_cast< QSize*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SliceWidget::*_t)(const QPoint & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SliceWidget::sliceSelected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SliceWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SliceWidget::sliceSelected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SliceWidget::*_t)(StrokeMarkItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SliceWidget::markAdded)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SliceWidget::*_t)(const QPointF & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SliceWidget::viewMoved)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (SliceWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SliceWidget::selectionChanged)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject SliceWidget::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_SliceWidget.data,
      qt_meta_data_SliceWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SliceWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SliceWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SliceWidget.stringdata0))
        return static_cast<void*>(const_cast< SliceWidget*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int SliceWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void SliceWidget::sliceSelected(const QPoint & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SliceWidget::sliceSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void SliceWidget::markAdded(StrokeMarkItem * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SliceWidget::viewMoved(const QPointF & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SliceWidget::selectionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
