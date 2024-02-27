/****************************************************************************
** Meta object code from reading C++ file 'PreprocessWin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/tomomain/widget/PreprocessWin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PreprocessWin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PreprocessWin_t {
    QByteArrayData data[16];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PreprocessWin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PreprocessWin_t qt_meta_stringdata_PreprocessWin = {
    {
QT_MOC_LITERAL(0, 0, 13), // "PreprocessWin"
QT_MOC_LITERAL(1, 14, 3), // "Log"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 7), // "message"
QT_MOC_LITERAL(4, 27, 6), // "Finish"
QT_MOC_LITERAL(5, 34, 4), // "step"
QT_MOC_LITERAL(6, 39, 8), // "RunError"
QT_MOC_LITERAL(7, 48, 5), // "Start"
QT_MOC_LITERAL(8, 54, 9), // "ShowInput"
QT_MOC_LITERAL(9, 64, 10), // "ShowOutput"
QT_MOC_LITERAL(10, 75, 14), // "StartNormalize"
QT_MOC_LITERAL(11, 90, 10), // "ScanHeader"
QT_MOC_LITERAL(12, 101, 11), // "StartRotate"
QT_MOC_LITERAL(13, 113, 9), // "UseInNext"
QT_MOC_LITERAL(14, 123, 15), // "ChangeAlgorithm"
QT_MOC_LITERAL(15, 139, 5) // "index"

    },
    "PreprocessWin\0Log\0\0message\0Finish\0"
    "step\0RunError\0Start\0ShowInput\0ShowOutput\0"
    "StartNormalize\0ScanHeader\0StartRotate\0"
    "UseInNext\0ChangeAlgorithm\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PreprocessWin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    1,   72,    2, 0x06 /* Public */,
       6,    0,   75,    2, 0x06 /* Public */,
       7,    0,   76,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   77,    2, 0x0a /* Public */,
       9,    0,   78,    2, 0x0a /* Public */,
      10,    0,   79,    2, 0x0a /* Public */,
      11,    1,   80,    2, 0x0a /* Public */,
      12,    1,   83,    2, 0x0a /* Public */,
      13,    1,   86,    2, 0x0a /* Public */,
      14,    1,   89,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,   15,

       0        // eod
};

void PreprocessWin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PreprocessWin *_t = static_cast<PreprocessWin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Log((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->Finish((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->RunError(); break;
        case 3: _t->Start(); break;
        case 4: _t->ShowInput(); break;
        case 5: _t->ShowOutput(); break;
        case 6: { bool _r = _t->StartNormalize();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: _t->ScanHeader((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->StartRotate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->UseInNext((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->ChangeAlgorithm((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PreprocessWin::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PreprocessWin::Log)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (PreprocessWin::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PreprocessWin::Finish)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (PreprocessWin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PreprocessWin::RunError)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (PreprocessWin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PreprocessWin::Start)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject PreprocessWin::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PreprocessWin.data,
      qt_meta_data_PreprocessWin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PreprocessWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PreprocessWin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PreprocessWin.stringdata0))
        return static_cast<void*>(const_cast< PreprocessWin*>(this));
    return QWidget::qt_metacast(_clname);
}

int PreprocessWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void PreprocessWin::Log(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PreprocessWin::Finish(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PreprocessWin::RunError()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void PreprocessWin::Start()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
