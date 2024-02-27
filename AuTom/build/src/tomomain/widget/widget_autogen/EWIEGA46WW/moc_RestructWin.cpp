/****************************************************************************
** Meta object code from reading C++ file 'RestructWin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/tomomain/widget/RestructWin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RestructWin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RestructWin_t {
    QByteArrayData data[18];
    char stringdata0[208];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RestructWin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RestructWin_t qt_meta_stringdata_RestructWin = {
    {
QT_MOC_LITERAL(0, 0, 11), // "RestructWin"
QT_MOC_LITERAL(1, 12, 3), // "Log"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 7), // "message"
QT_MOC_LITERAL(4, 25, 6), // "Finish"
QT_MOC_LITERAL(5, 32, 4), // "step"
QT_MOC_LITERAL(6, 37, 5), // "Start"
QT_MOC_LITERAL(7, 43, 8), // "RunError"
QT_MOC_LITERAL(8, 52, 9), // "ShowInput"
QT_MOC_LITERAL(9, 62, 10), // "ShowOutput"
QT_MOC_LITERAL(10, 73, 13), // "StartRestruct"
QT_MOC_LITERAL(11, 87, 16), // "StartforRestruct"
QT_MOC_LITERAL(12, 104, 13), // "SetParVisible"
QT_MOC_LITERAL(13, 118, 15), // "ChangeAlgorithm"
QT_MOC_LITERAL(14, 134, 16), // "ChangeAlgorithm2"
QT_MOC_LITERAL(15, 151, 10), // "ChooseFile"
QT_MOC_LITERAL(16, 162, 19), // "EnableCTFCorrection"
QT_MOC_LITERAL(17, 182, 25) // "ShowCTFCorrectionSettings"

    },
    "RestructWin\0Log\0\0message\0Finish\0step\0"
    "Start\0RunError\0ShowInput\0ShowOutput\0"
    "StartRestruct\0StartforRestruct\0"
    "SetParVisible\0ChangeAlgorithm\0"
    "ChangeAlgorithm2\0ChooseFile\0"
    "EnableCTFCorrection\0ShowCTFCorrectionSettings"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RestructWin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    1,   87,    2, 0x06 /* Public */,
       6,    0,   90,    2, 0x06 /* Public */,
       7,    0,   91,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   92,    2, 0x0a /* Public */,
       9,    0,   93,    2, 0x0a /* Public */,
      10,    0,   94,    2, 0x0a /* Public */,
      11,    0,   95,    2, 0x0a /* Public */,
      12,    0,   96,    2, 0x0a /* Public */,
      13,    0,   97,    2, 0x0a /* Public */,
      14,    0,   98,    2, 0x0a /* Public */,
      15,    0,   99,    2, 0x0a /* Public */,
      16,    0,  100,    2, 0x0a /* Public */,
      17,    0,  101,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RestructWin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RestructWin *_t = static_cast<RestructWin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Log((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->Finish((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->Start(); break;
        case 3: _t->RunError(); break;
        case 4: _t->ShowInput(); break;
        case 5: _t->ShowOutput(); break;
        case 6: { int _r = _t->StartRestruct();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: { int _r = _t->StartforRestruct();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 8: _t->SetParVisible(); break;
        case 9: _t->ChangeAlgorithm(); break;
        case 10: _t->ChangeAlgorithm2(); break;
        case 11: _t->ChooseFile(); break;
        case 12: _t->EnableCTFCorrection(); break;
        case 13: _t->ShowCTFCorrectionSettings(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RestructWin::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RestructWin::Log)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RestructWin::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RestructWin::Finish)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RestructWin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RestructWin::Start)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RestructWin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RestructWin::RunError)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject RestructWin::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RestructWin.data,
      qt_meta_data_RestructWin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RestructWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RestructWin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RestructWin.stringdata0))
        return static_cast<void*>(const_cast< RestructWin*>(this));
    return QWidget::qt_metacast(_clname);
}

int RestructWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void RestructWin::Log(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RestructWin::Finish(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RestructWin::Start()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void RestructWin::RunError()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
