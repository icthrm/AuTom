/****************************************************************************
** Meta object code from reading C++ file 'CoarseAlignWin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/tomomain/widget/CoarseAlignWin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CoarseAlignWin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CoarseAlignWin_t {
    QByteArrayData data[11];
    char stringdata0[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CoarseAlignWin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CoarseAlignWin_t qt_meta_stringdata_CoarseAlignWin = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CoarseAlignWin"
QT_MOC_LITERAL(1, 15, 3), // "Log"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 7), // "message"
QT_MOC_LITERAL(4, 28, 6), // "Finish"
QT_MOC_LITERAL(5, 35, 4), // "step"
QT_MOC_LITERAL(6, 40, 8), // "RunError"
QT_MOC_LITERAL(7, 49, 5), // "Start"
QT_MOC_LITERAL(8, 55, 9), // "ShowInput"
QT_MOC_LITERAL(9, 65, 10), // "ShowOutput"
QT_MOC_LITERAL(10, 76, 16) // "StartCoarseAlign"

    },
    "CoarseAlignWin\0Log\0\0message\0Finish\0"
    "step\0RunError\0Start\0ShowInput\0ShowOutput\0"
    "StartCoarseAlign"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CoarseAlignWin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    1,   52,    2, 0x06 /* Public */,
       6,    0,   55,    2, 0x06 /* Public */,
       7,    0,   56,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   57,    2, 0x0a /* Public */,
       9,    0,   58,    2, 0x0a /* Public */,
      10,    0,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CoarseAlignWin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CoarseAlignWin *_t = static_cast<CoarseAlignWin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Log((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->Finish((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->RunError(); break;
        case 3: _t->Start(); break;
        case 4: _t->ShowInput(); break;
        case 5: _t->ShowOutput(); break;
        case 6: _t->StartCoarseAlign(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CoarseAlignWin::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoarseAlignWin::Log)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CoarseAlignWin::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoarseAlignWin::Finish)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CoarseAlignWin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoarseAlignWin::RunError)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CoarseAlignWin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CoarseAlignWin::Start)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject CoarseAlignWin::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CoarseAlignWin.data,
      qt_meta_data_CoarseAlignWin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CoarseAlignWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CoarseAlignWin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CoarseAlignWin.stringdata0))
        return static_cast<void*>(const_cast< CoarseAlignWin*>(this));
    return QWidget::qt_metacast(_clname);
}

int CoarseAlignWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CoarseAlignWin::Log(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CoarseAlignWin::Finish(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CoarseAlignWin::RunError()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void CoarseAlignWin::Start()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
