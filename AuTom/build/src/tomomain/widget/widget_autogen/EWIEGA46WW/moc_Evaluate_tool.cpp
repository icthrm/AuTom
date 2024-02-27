/****************************************************************************
** Meta object code from reading C++ file 'Evaluate_tool.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/tomomain/widget/Evaluate_tool.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Evaluate_tool.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CTrajPlotWin_t {
    QByteArrayData data[6];
    char stringdata0[54];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CTrajPlotWin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CTrajPlotWin_t qt_meta_stringdata_CTrajPlotWin = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CTrajPlotWin"
QT_MOC_LITERAL(1, 13, 5), // "start"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 7), // "message"
QT_MOC_LITERAL(4, 28, 12), // "rawOrAligned"
QT_MOC_LITERAL(5, 41, 12) // "startProcess"

    },
    "CTrajPlotWin\0start\0\0message\0rawOrAligned\0"
    "startProcess"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CTrajPlotWin[] = {

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
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   29,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CTrajPlotWin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CTrajPlotWin *_t = static_cast<CTrajPlotWin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->start((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->startProcess(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CTrajPlotWin::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CTrajPlotWin::start)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CTrajPlotWin::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CTrajPlotWin.data,
      qt_meta_data_CTrajPlotWin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CTrajPlotWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CTrajPlotWin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CTrajPlotWin.stringdata0))
        return static_cast<void*>(const_cast< CTrajPlotWin*>(this));
    return QWidget::qt_metacast(_clname);
}

int CTrajPlotWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CTrajPlotWin::start(QString _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_Evaluate_tool_t {
    QByteArrayData data[9];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Evaluate_tool_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Evaluate_tool_t qt_meta_stringdata_Evaluate_tool = {
    {
QT_MOC_LITERAL(0, 0, 13), // "Evaluate_tool"
QT_MOC_LITERAL(1, 14, 3), // "Log"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 7), // "message"
QT_MOC_LITERAL(4, 27, 3), // "end"
QT_MOC_LITERAL(5, 31, 13), // "StartEvaluate"
QT_MOC_LITERAL(6, 45, 21), // "StartTrajplotEvaluate"
QT_MOC_LITERAL(7, 67, 12), // "rawOrAligned"
QT_MOC_LITERAL(8, 80, 4) // "kill"

    },
    "Evaluate_tool\0Log\0\0message\0end\0"
    "StartEvaluate\0StartTrajplotEvaluate\0"
    "rawOrAligned\0kill"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Evaluate_tool[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    0,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   43,    2, 0x0a /* Public */,
       6,    2,   44,    2, 0x0a /* Public */,
       8,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    7,
    QMetaType::Void,

       0        // eod
};

void Evaluate_tool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Evaluate_tool *_t = static_cast<Evaluate_tool *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Log((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->end(); break;
        case 2: _t->StartEvaluate(); break;
        case 3: _t->StartTrajplotEvaluate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->kill(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Evaluate_tool::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Evaluate_tool::Log)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Evaluate_tool::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Evaluate_tool::end)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject Evaluate_tool::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Evaluate_tool.data,
      qt_meta_data_Evaluate_tool,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Evaluate_tool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Evaluate_tool::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Evaluate_tool.stringdata0))
        return static_cast<void*>(const_cast< Evaluate_tool*>(this));
    return QWidget::qt_metacast(_clname);
}

int Evaluate_tool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Evaluate_tool::Log(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Evaluate_tool::end()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
