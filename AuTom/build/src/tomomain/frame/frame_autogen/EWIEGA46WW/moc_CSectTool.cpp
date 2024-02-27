/****************************************************************************
** Meta object code from reading C++ file 'CSectTool.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/tomomain/frame/CSectTool.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CSectTool.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CSectTool_t {
    QByteArrayData data[8];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CSectTool_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CSectTool_t qt_meta_stringdata_CSectTool = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CSectTool"
QT_MOC_LITERAL(1, 10, 12), // "ShowSectView"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 10), // "ShowSlice1"
QT_MOC_LITERAL(4, 35, 10), // "ShowSlice2"
QT_MOC_LITERAL(5, 46, 10), // "ShowSlice3"
QT_MOC_LITERAL(6, 57, 11), // "UpdateScene"
QT_MOC_LITERAL(7, 69, 19) // "UpdateSliceSkeleton"

    },
    "CSectTool\0ShowSectView\0\0ShowSlice1\0"
    "ShowSlice2\0ShowSlice3\0UpdateScene\0"
    "UpdateSliceSkeleton"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CSectTool[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    0,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CSectTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CSectTool *_t = static_cast<CSectTool *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ShowSectView(); break;
        case 1: _t->ShowSlice1(); break;
        case 2: _t->ShowSlice2(); break;
        case 3: _t->ShowSlice3(); break;
        case 4: _t->UpdateScene(); break;
        case 5: _t->UpdateSliceSkeleton(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CSectTool::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSectTool.data,
      qt_meta_data_CSectTool,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CSectTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CSectTool::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CSectTool.stringdata0))
        return static_cast<void*>(const_cast< CSectTool*>(this));
    return QWidget::qt_metacast(_clname);
}

int CSectTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_CSectToolFrame_t {
    QByteArrayData data[27];
    char stringdata0[294];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CSectToolFrame_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CSectToolFrame_t qt_meta_stringdata_CSectToolFrame = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CSectToolFrame"
QT_MOC_LITERAL(1, 15, 6), // "Finish"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 4), // "step"
QT_MOC_LITERAL(4, 28, 5), // "Start"
QT_MOC_LITERAL(5, 34, 3), // "Log"
QT_MOC_LITERAL(6, 38, 7), // "message"
QT_MOC_LITERAL(7, 46, 8), // "RunError"
QT_MOC_LITERAL(8, 55, 13), // "DoCalibration"
QT_MOC_LITERAL(9, 69, 9), // "writeFile"
QT_MOC_LITERAL(10, 79, 7), // "z_shift"
QT_MOC_LITERAL(11, 87, 6), // "offset"
QT_MOC_LITERAL(12, 94, 9), // "thickness"
QT_MOC_LITERAL(13, 104, 11), // "pitch_angle"
QT_MOC_LITERAL(14, 116, 11), // "inputmethod"
QT_MOC_LITERAL(15, 128, 13), // "writeFile_geo"
QT_MOC_LITERAL(16, 142, 14), // "finishGeometry"
QT_MOC_LITERAL(17, 157, 13), // "AutoDemarcate"
QT_MOC_LITERAL(18, 171, 17), // "StopAutoDemarcate"
QT_MOC_LITERAL(19, 189, 12), // "SetSetWindow"
QT_MOC_LITERAL(20, 202, 7), // "Setting"
QT_MOC_LITERAL(21, 210, 16), // "QAbstractButton*"
QT_MOC_LITERAL(22, 227, 6), // "button"
QT_MOC_LITERAL(23, 234, 17), // "ChangeinputMethod"
QT_MOC_LITERAL(24, 252, 12), // "inputProcess"
QT_MOC_LITERAL(25, 265, 22), // "ChangeinputMethod_auto"
QT_MOC_LITERAL(26, 288, 5) // "AutoG"

    },
    "CSectToolFrame\0Finish\0\0step\0Start\0Log\0"
    "message\0RunError\0DoCalibration\0writeFile\0"
    "z_shift\0offset\0thickness\0pitch_angle\0"
    "inputmethod\0writeFile_geo\0finishGeometry\0"
    "AutoDemarcate\0StopAutoDemarcate\0"
    "SetSetWindow\0Setting\0QAbstractButton*\0"
    "button\0ChangeinputMethod\0inputProcess\0"
    "ChangeinputMethod_auto\0AutoG"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CSectToolFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,
       4,    0,   97,    2, 0x06 /* Public */,
       5,    1,   98,    2, 0x06 /* Public */,
       7,    0,  101,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,  102,    2, 0x08 /* Private */,
       9,    5,  103,    2, 0x08 /* Private */,
      15,    0,  114,    2, 0x08 /* Private */,
      16,    0,  115,    2, 0x08 /* Private */,
      17,    0,  116,    2, 0x08 /* Private */,
      18,    0,  117,    2, 0x08 /* Private */,
      19,    0,  118,    2, 0x08 /* Private */,
      20,    1,  119,    2, 0x08 /* Private */,
      23,    0,  122,    2, 0x08 /* Private */,
      24,    0,  123,    2, 0x08 /* Private */,
      25,    0,  124,    2, 0x08 /* Private */,
      26,    0,  125,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Int,   10,   11,   12,   13,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CSectToolFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CSectToolFrame *_t = static_cast<CSectToolFrame *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Finish((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->Start(); break;
        case 2: _t->Log((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->RunError(); break;
        case 4: _t->DoCalibration(); break;
        case 5: _t->writeFile((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 6: _t->writeFile_geo(); break;
        case 7: _t->finishGeometry(); break;
        case 8: _t->AutoDemarcate(); break;
        case 9: _t->StopAutoDemarcate(); break;
        case 10: _t->SetSetWindow(); break;
        case 11: _t->Setting((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 12: _t->ChangeinputMethod(); break;
        case 13: _t->inputProcess(); break;
        case 14: _t->ChangeinputMethod_auto(); break;
        case 15: _t->AutoG(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CSectToolFrame::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CSectToolFrame::Finish)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CSectToolFrame::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CSectToolFrame::Start)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CSectToolFrame::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CSectToolFrame::Log)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CSectToolFrame::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CSectToolFrame::RunError)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject CSectToolFrame::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CSectToolFrame.data,
      qt_meta_data_CSectToolFrame,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CSectToolFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CSectToolFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CSectToolFrame.stringdata0))
        return static_cast<void*>(const_cast< CSectToolFrame*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CSectToolFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void CSectToolFrame::Finish(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CSectToolFrame::Start()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void CSectToolFrame::Log(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CSectToolFrame::RunError()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
