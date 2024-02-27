/****************************************************************************
** Meta object code from reading C++ file 'titledsliderwithspinbox.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../src/tomomain/ImageViewWindow/widgets/titledsliderwithspinbox.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'titledsliderwithspinbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TitledSliderWithSpinBox_t {
    QByteArrayData data[5];
    char stringdata0[53];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TitledSliderWithSpinBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TitledSliderWithSpinBox_t qt_meta_stringdata_TitledSliderWithSpinBox = {
    {
QT_MOC_LITERAL(0, 0, 23), // "TitledSliderWithSpinBox"
QT_MOC_LITERAL(1, 24, 12), // "valueChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 5), // "value"
QT_MOC_LITERAL(4, 44, 8) // "setValue"

    },
    "TitledSliderWithSpinBox\0valueChanged\0"
    "\0value\0setValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TitledSliderWithSpinBox[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void TitledSliderWithSpinBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TitledSliderWithSpinBox *_t = static_cast<TitledSliderWithSpinBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TitledSliderWithSpinBox::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TitledSliderWithSpinBox::valueChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject TitledSliderWithSpinBox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TitledSliderWithSpinBox.data,
      qt_meta_data_TitledSliderWithSpinBox,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TitledSliderWithSpinBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TitledSliderWithSpinBox::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TitledSliderWithSpinBox.stringdata0))
        return static_cast<void*>(const_cast< TitledSliderWithSpinBox*>(this));
    return QWidget::qt_metacast(_clname);
}

int TitledSliderWithSpinBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void TitledSliderWithSpinBox::valueChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_TitledSliderWidthDoubleSpinBox_t {
    QByteArrayData data[4];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TitledSliderWidthDoubleSpinBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TitledSliderWidthDoubleSpinBox_t qt_meta_stringdata_TitledSliderWidthDoubleSpinBox = {
    {
QT_MOC_LITERAL(0, 0, 30), // "TitledSliderWidthDoubleSpinBox"
QT_MOC_LITERAL(1, 31, 12), // "valueChanged"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 5) // "value"

    },
    "TitledSliderWidthDoubleSpinBox\0"
    "valueChanged\0\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TitledSliderWidthDoubleSpinBox[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,

       0        // eod
};

void TitledSliderWidthDoubleSpinBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TitledSliderWidthDoubleSpinBox *_t = static_cast<TitledSliderWidthDoubleSpinBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TitledSliderWidthDoubleSpinBox::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TitledSliderWidthDoubleSpinBox::valueChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject TitledSliderWidthDoubleSpinBox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TitledSliderWidthDoubleSpinBox.data,
      qt_meta_data_TitledSliderWidthDoubleSpinBox,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TitledSliderWidthDoubleSpinBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TitledSliderWidthDoubleSpinBox::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TitledSliderWidthDoubleSpinBox.stringdata0))
        return static_cast<void*>(const_cast< TitledSliderWidthDoubleSpinBox*>(this));
    return QWidget::qt_metacast(_clname);
}

int TitledSliderWidthDoubleSpinBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void TitledSliderWidthDoubleSpinBox::valueChanged(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
