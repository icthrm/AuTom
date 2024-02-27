/****************************************************************************
** Meta object code from reading C++ file 'renderoptionwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../src/tomomain/ImageViewWindow/widgets/renderoptionwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'renderoptionwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RenderParameterWidget_t {
    QByteArrayData data[16];
    char stringdata0[235];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RenderParameterWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RenderParameterWidget_t qt_meta_stringdata_RenderParameterWidget = {
    {
QT_MOC_LITERAL(0, 0, 21), // "RenderParameterWidget"
QT_MOC_LITERAL(1, 22, 14), // "optionsChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 11), // "markUpdated"
QT_MOC_LITERAL(4, 50, 24), // "transferFunctionsChanged"
QT_MOC_LITERAL(5, 75, 12), // "const float*"
QT_MOC_LITERAL(6, 88, 5), // "funcs"
QT_MOC_LITERAL(7, 94, 15), // "updateDataModel"
QT_MOC_LITERAL(8, 110, 19), // "radialSliderChanged"
QT_MOC_LITERAL(9, 130, 5), // "value"
QT_MOC_LITERAL(10, 136, 18), // "thetaSliderChanged"
QT_MOC_LITERAL(11, 155, 16), // "phiSliderChanged"
QT_MOC_LITERAL(12, 172, 23), // "transferFunctionChanged"
QT_MOC_LITERAL(13, 196, 17), // "renderTypeChanged"
QT_MOC_LITERAL(14, 214, 4), // "text"
QT_MOC_LITERAL(15, 219, 15) // "tfButtonClicked"

    },
    "RenderParameterWidget\0optionsChanged\0"
    "\0markUpdated\0transferFunctionsChanged\0"
    "const float*\0funcs\0updateDataModel\0"
    "radialSliderChanged\0value\0thetaSliderChanged\0"
    "phiSliderChanged\0transferFunctionChanged\0"
    "renderTypeChanged\0text\0tfButtonClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RenderParameterWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    1,   66,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   69,    2, 0x08 /* Private */,
       8,    1,   70,    2, 0x08 /* Private */,
      10,    1,   73,    2, 0x08 /* Private */,
      11,    1,   76,    2, 0x08 /* Private */,
      12,    0,   79,    2, 0x08 /* Private */,
      13,    1,   80,    2, 0x08 /* Private */,
      15,    0,   83,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,

       0        // eod
};

void RenderParameterWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RenderParameterWidget *_t = static_cast<RenderParameterWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->optionsChanged(); break;
        case 1: _t->markUpdated(); break;
        case 2: _t->transferFunctionsChanged((*reinterpret_cast< const float*(*)>(_a[1]))); break;
        case 3: _t->updateDataModel(); break;
        case 4: _t->radialSliderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->thetaSliderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->phiSliderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->transferFunctionChanged(); break;
        case 8: _t->renderTypeChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->tfButtonClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RenderParameterWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RenderParameterWidget::optionsChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RenderParameterWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RenderParameterWidget::markUpdated)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RenderParameterWidget::*_t)(const float * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RenderParameterWidget::transferFunctionsChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject RenderParameterWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RenderParameterWidget.data,
      qt_meta_data_RenderParameterWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RenderParameterWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RenderParameterWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RenderParameterWidget.stringdata0))
        return static_cast<void*>(const_cast< RenderParameterWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int RenderParameterWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void RenderParameterWidget::optionsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void RenderParameterWidget::markUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void RenderParameterWidget::transferFunctionsChanged(const float * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
