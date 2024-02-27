/****************************************************************************
** Meta object code from reading C++ file 'slicevolume.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../../src/tomomain/ImageViewWindow/3drender/geometry/slicevolume.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'slicevolume.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SliceVolume_t {
    QByteArrayData data[5];
    char stringdata0[36];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SliceVolume_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SliceVolume_t qt_meta_stringdata_SliceVolume = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SliceVolume"
QT_MOC_LITERAL(1, 12, 18), // "setFramebufferSize"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 1), // "w"
QT_MOC_LITERAL(4, 34, 1) // "h"

    },
    "SliceVolume\0setFramebufferSize\0\0w\0h"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SliceVolume[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,

       0        // eod
};

void SliceVolume::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SliceVolume *_t = static_cast<SliceVolume *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setFramebufferSize((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject SliceVolume::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SliceVolume.data,
      qt_meta_data_SliceVolume,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SliceVolume::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SliceVolume::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SliceVolume.stringdata0))
        return static_cast<void*>(const_cast< SliceVolume*>(this));
    if (!strcmp(_clname, "GPUVolume"))
        return static_cast< GPUVolume*>(const_cast< SliceVolume*>(this));
    if (!strcmp(_clname, "ShaderDataInterface"))
        return static_cast< ShaderDataInterface*>(const_cast< SliceVolume*>(this));
    return QObject::qt_metacast(_clname);
}

int SliceVolume::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
