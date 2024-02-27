/****************************************************************************
** Meta object code from reading C++ file 'AlignWin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/tomomain/widget/AlignWin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AlignWin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AlignWin_t {
    QByteArrayData data[12];
    char stringdata0[98];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AlignWin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AlignWin_t qt_meta_stringdata_AlignWin = {
    {
QT_MOC_LITERAL(0, 0, 8), // "AlignWin"
QT_MOC_LITERAL(1, 9, 3), // "Log"
QT_MOC_LITERAL(2, 13, 0), // ""
QT_MOC_LITERAL(3, 14, 7), // "message"
QT_MOC_LITERAL(4, 22, 6), // "Finish"
QT_MOC_LITERAL(5, 29, 4), // "step"
QT_MOC_LITERAL(6, 34, 5), // "Start"
QT_MOC_LITERAL(7, 40, 8), // "RunError"
QT_MOC_LITERAL(8, 49, 9), // "ShowInput"
QT_MOC_LITERAL(9, 59, 10), // "ShowOutput"
QT_MOC_LITERAL(10, 70, 11), // "ViewFidMode"
QT_MOC_LITERAL(11, 82, 15) // "StartMarkerAuto"

    },
    "AlignWin\0Log\0\0message\0Finish\0step\0"
    "Start\0RunError\0ShowInput\0ShowOutput\0"
    "ViewFidMode\0StartMarkerAuto"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AlignWin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       6,    0,   60,    2, 0x06 /* Public */,
       7,    0,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   62,    2, 0x0a /* Public */,
       9,    0,   63,    2, 0x0a /* Public */,
      10,    1,   64,    2, 0x0a /* Public */,
      11,    0,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,

       0        // eod
};

void AlignWin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AlignWin *_t = static_cast<AlignWin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Log((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->Finish((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->Start(); break;
        case 3: _t->RunError(); break;
        case 4: _t->ShowInput(); break;
        case 5: _t->ShowOutput(); break;
        case 6: _t->ViewFidMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->StartMarkerAuto(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AlignWin::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AlignWin::Log)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AlignWin::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AlignWin::Finish)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AlignWin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AlignWin::Start)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AlignWin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AlignWin::RunError)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject AlignWin::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AlignWin.data,
      qt_meta_data_AlignWin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AlignWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AlignWin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AlignWin.stringdata0))
        return static_cast<void*>(const_cast< AlignWin*>(this));
    return QWidget::qt_metacast(_clname);
}

int AlignWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void AlignWin::Log(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AlignWin::Finish(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AlignWin::Start()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void AlignWin::RunError()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
