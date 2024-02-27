/****************************************************************************
** Meta object code from reading C++ file 'FineAlignWin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/tomomain/widget/FineAlignWin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FineAlignWin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FineAlignWin_t {
    QByteArrayData data[14];
    char stringdata0[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FineAlignWin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FineAlignWin_t qt_meta_stringdata_FineAlignWin = {
    {
QT_MOC_LITERAL(0, 0, 12), // "FineAlignWin"
QT_MOC_LITERAL(1, 13, 3), // "Log"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 7), // "message"
QT_MOC_LITERAL(4, 26, 6), // "Finish"
QT_MOC_LITERAL(5, 33, 4), // "step"
QT_MOC_LITERAL(6, 38, 5), // "Start"
QT_MOC_LITERAL(7, 44, 8), // "RunError"
QT_MOC_LITERAL(8, 53, 9), // "ShowInput"
QT_MOC_LITERAL(9, 63, 10), // "ShowOutput"
QT_MOC_LITERAL(10, 74, 18), // "StartMarklessAlign"
QT_MOC_LITERAL(11, 93, 9), // "SetButton"
QT_MOC_LITERAL(12, 103, 3), // "txt"
QT_MOC_LITERAL(13, 107, 11) // "ViewFidMode"

    },
    "FineAlignWin\0Log\0\0message\0Finish\0step\0"
    "Start\0RunError\0ShowInput\0ShowOutput\0"
    "StartMarklessAlign\0SetButton\0txt\0"
    "ViewFidMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FineAlignWin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    1,   62,    2, 0x06 /* Public */,
       6,    0,   65,    2, 0x06 /* Public */,
       7,    0,   66,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   67,    2, 0x0a /* Public */,
       9,    0,   68,    2, 0x0a /* Public */,
      10,    0,   69,    2, 0x0a /* Public */,
      11,    1,   70,    2, 0x0a /* Public */,
      13,    1,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void FineAlignWin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FineAlignWin *_t = static_cast<FineAlignWin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Log((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->Finish((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->Start(); break;
        case 3: _t->RunError(); break;
        case 4: _t->ShowInput(); break;
        case 5: _t->ShowOutput(); break;
        case 6: _t->StartMarklessAlign(); break;
        case 7: _t->SetButton((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->ViewFidMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FineAlignWin::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FineAlignWin::Log)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (FineAlignWin::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FineAlignWin::Finish)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (FineAlignWin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FineAlignWin::Start)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (FineAlignWin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FineAlignWin::RunError)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject FineAlignWin::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FineAlignWin.data,
      qt_meta_data_FineAlignWin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FineAlignWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FineAlignWin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FineAlignWin.stringdata0))
        return static_cast<void*>(const_cast< FineAlignWin*>(this));
    return QWidget::qt_metacast(_clname);
}

int FineAlignWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void FineAlignWin::Log(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FineAlignWin::Finish(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FineAlignWin::Start()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void FineAlignWin::RunError()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
