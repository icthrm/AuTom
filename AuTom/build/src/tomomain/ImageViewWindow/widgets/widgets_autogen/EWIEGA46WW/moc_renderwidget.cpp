/****************************************************************************
** Meta object code from reading C++ file 'renderwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../src/tomomain/ImageViewWindow/widgets/renderwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'renderwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RenderWidget_t {
    QByteArrayData data[33];
    char stringdata0[461];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RenderWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RenderWidget_t qt_meta_stringdata_RenderWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "RenderWidget"
QT_MOC_LITERAL(1, 13, 16), // "markModelChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 16), // "dataModelChanged"
QT_MOC_LITERAL(4, 48, 23), // "requireTransferFunction"
QT_MOC_LITERAL(5, 72, 13), // "windowResized"
QT_MOC_LITERAL(6, 86, 1), // "w"
QT_MOC_LITERAL(7, 88, 1), // "h"
QT_MOC_LITERAL(8, 90, 22), // "updateTransferFunction"
QT_MOC_LITERAL(9, 113, 12), // "const float*"
QT_MOC_LITERAL(10, 126, 4), // "func"
QT_MOC_LITERAL(11, 131, 11), // "setTopSlice"
QT_MOC_LITERAL(12, 143, 5), // "value"
QT_MOC_LITERAL(13, 149, 13), // "setRightSlice"
QT_MOC_LITERAL(14, 163, 13), // "setFrontSlice"
QT_MOC_LITERAL(15, 177, 18), // "setTopSliceVisible"
QT_MOC_LITERAL(16, 196, 5), // "check"
QT_MOC_LITERAL(17, 202, 20), // "setRightSliceVisible"
QT_MOC_LITERAL(18, 223, 20), // "setFrontSliceVisible"
QT_MOC_LITERAL(19, 244, 10), // "updateMark"
QT_MOC_LITERAL(20, 255, 26), // "_slot_markModelDataChanged"
QT_MOC_LITERAL(21, 282, 5), // "begin"
QT_MOC_LITERAL(22, 288, 3), // "end"
QT_MOC_LITERAL(23, 292, 12), // "QVector<int>"
QT_MOC_LITERAL(24, 305, 4), // "role"
QT_MOC_LITERAL(25, 310, 24), // "_slot_currentMeshChanged"
QT_MOC_LITERAL(26, 335, 7), // "current"
QT_MOC_LITERAL(27, 343, 8), // "previous"
QT_MOC_LITERAL(28, 352, 35), // "_slot_currentChanged_selectio..."
QT_MOC_LITERAL(29, 388, 37), // "_slot_selectionChanged_select..."
QT_MOC_LITERAL(30, 426, 14), // "QItemSelection"
QT_MOC_LITERAL(31, 441, 8), // "selected"
QT_MOC_LITERAL(32, 450, 10) // "deselected"

    },
    "RenderWidget\0markModelChanged\0\0"
    "dataModelChanged\0requireTransferFunction\0"
    "windowResized\0w\0h\0updateTransferFunction\0"
    "const float*\0func\0setTopSlice\0value\0"
    "setRightSlice\0setFrontSlice\0"
    "setTopSliceVisible\0check\0setRightSliceVisible\0"
    "setFrontSliceVisible\0updateMark\0"
    "_slot_markModelDataChanged\0begin\0end\0"
    "QVector<int>\0role\0_slot_currentMeshChanged\0"
    "current\0previous\0_slot_currentChanged_selectionModel\0"
    "_slot_selectionChanged_selectionModel\0"
    "QItemSelection\0selected\0deselected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RenderWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x06 /* Public */,
       3,    0,   95,    2, 0x06 /* Public */,
       4,    0,   96,    2, 0x06 /* Public */,
       5,    2,   97,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,  102,    2, 0x0a /* Public */,
      11,    1,  105,    2, 0x0a /* Public */,
      13,    1,  108,    2, 0x0a /* Public */,
      14,    1,  111,    2, 0x0a /* Public */,
      15,    1,  114,    2, 0x0a /* Public */,
      17,    1,  117,    2, 0x0a /* Public */,
      18,    1,  120,    2, 0x0a /* Public */,
      19,    0,  123,    2, 0x08 /* Private */,
      20,    3,  124,    2, 0x08 /* Private */,
      25,    2,  131,    2, 0x08 /* Private */,
      28,    2,  136,    2, 0x08 /* Private */,
      29,    2,  141,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex, QMetaType::QModelIndex, 0x80000000 | 23,   21,   22,   24,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   26,   27,
    QMetaType::Void, QMetaType::QModelIndex, QMetaType::QModelIndex,   26,   27,
    QMetaType::Void, 0x80000000 | 30, 0x80000000 | 30,   31,   32,

       0        // eod
};

void RenderWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RenderWidget *_t = static_cast<RenderWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->markModelChanged(); break;
        case 1: _t->dataModelChanged(); break;
        case 2: _t->requireTransferFunction(); break;
        case 3: _t->windowResized((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->updateTransferFunction((*reinterpret_cast< const float*(*)>(_a[1]))); break;
        case 5: _t->setTopSlice((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setRightSlice((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setFrontSlice((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->setTopSliceVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->setRightSliceVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->setFrontSliceVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->updateMark(); break;
        case 12: _t->_slot_markModelDataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2])),(*reinterpret_cast< const QVector<int>(*)>(_a[3]))); break;
        case 13: _t->_slot_currentMeshChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->_slot_currentChanged_selectionModel((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 15: _t->_slot_selectionChanged_selectionModel((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<int> >(); break;
            }
            break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QItemSelection >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RenderWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RenderWidget::markModelChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RenderWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RenderWidget::dataModelChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RenderWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RenderWidget::requireTransferFunction)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RenderWidget::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RenderWidget::windowResized)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject RenderWidget::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_RenderWidget.data,
      qt_meta_data_RenderWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RenderWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RenderWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RenderWidget.stringdata0))
        return static_cast<void*>(const_cast< RenderWidget*>(this));
    if (!strcmp(_clname, "QOpenGLFunctions_3_3_Core"))
        return static_cast< QOpenGLFunctions_3_3_Core*>(const_cast< RenderWidget*>(this));
    return QOpenGLWidget::qt_metacast(_clname);
}

int RenderWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void RenderWidget::markModelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void RenderWidget::dataModelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void RenderWidget::requireTransferFunction()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void RenderWidget::windowResized(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
