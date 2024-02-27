/****************************************************************************
** Meta object code from reading C++ file 'doubleslider.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../src/tomomain/ImageViewWindow/widgets/doubleslider.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'doubleslider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DoubleSlider_t {
    QByteArrayData data[12];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DoubleSlider_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DoubleSlider_t qt_meta_stringdata_DoubleSlider = {
    {
QT_MOC_LITERAL(0, 0, 12), // "DoubleSlider"
QT_MOC_LITERAL(1, 13, 13), // "valuesChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 3), // "min"
QT_MOC_LITERAL(4, 32, 3), // "max"
QT_MOC_LITERAL(5, 36, 17), // "toggleInteraction"
QT_MOC_LITERAL(6, 54, 2), // "on"
QT_MOC_LITERAL(7, 57, 11), // "setMaxValue"
QT_MOC_LITERAL(8, 69, 11), // "setMinValue"
QT_MOC_LITERAL(9, 81, 9), // "setValues"
QT_MOC_LITERAL(10, 91, 14), // "setSliderWidth"
QT_MOC_LITERAL(11, 106, 7) // "sliderW"

    },
    "DoubleSlider\0valuesChanged\0\0min\0max\0"
    "toggleInteraction\0on\0setMaxValue\0"
    "setMinValue\0setValues\0setSliderWidth\0"
    "sliderW"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DoubleSlider[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,
       5,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   52,    2, 0x0a /* Public */,
       8,    1,   55,    2, 0x0a /* Public */,
       9,    2,   58,    2, 0x0a /* Public */,
      10,    1,   63,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    3,    4,
    QMetaType::Void, QMetaType::Bool,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,    4,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    3,    4,
    QMetaType::Void, QMetaType::Int,   11,

       0        // eod
};

void DoubleSlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DoubleSlider *_t = static_cast<DoubleSlider *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valuesChanged((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 1: _t->toggleInteraction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->setMaxValue((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->setMinValue((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 4: _t->setValues((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 5: _t->setSliderWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DoubleSlider::*_t)(float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DoubleSlider::valuesChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DoubleSlider::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DoubleSlider::toggleInteraction)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject DoubleSlider::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DoubleSlider.data,
      qt_meta_data_DoubleSlider,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DoubleSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DoubleSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DoubleSlider.stringdata0))
        return static_cast<void*>(const_cast< DoubleSlider*>(this));
    return QWidget::qt_metacast(_clname);
}

int DoubleSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void DoubleSlider::valuesChanged(float _t1, float _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DoubleSlider::toggleInteraction(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
