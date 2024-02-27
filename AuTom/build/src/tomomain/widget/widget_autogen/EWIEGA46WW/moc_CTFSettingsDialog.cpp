/****************************************************************************
** Meta object code from reading C++ file 'CTFSettingsDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/tomomain/widget/CTFSettingsDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CTFSettingsDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CTFSettingsDialog_t {
    QByteArrayData data[12];
    char stringdata0[135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CTFSettingsDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CTFSettingsDialog_t qt_meta_stringdata_CTFSettingsDialog = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CTFSettingsDialog"
QT_MOC_LITERAL(1, 18, 3), // "Log"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 7), // "message"
QT_MOC_LITERAL(4, 31, 6), // "Finish"
QT_MOC_LITERAL(5, 38, 5), // "Start"
QT_MOC_LITERAL(6, 44, 8), // "RunError"
QT_MOC_LITERAL(7, 53, 15), // "enableShellEdit"
QT_MOC_LITERAL(8, 69, 25), // "enableAmplitudeCorrection"
QT_MOC_LITERAL(9, 95, 13), // "generateShell"
QT_MOC_LITERAL(10, 109, 12), // "executeShell"
QT_MOC_LITERAL(11, 122, 12) // "printMessage"

    },
    "CTFSettingsDialog\0Log\0\0message\0Finish\0"
    "Start\0RunError\0enableShellEdit\0"
    "enableAmplitudeCorrection\0generateShell\0"
    "executeShell\0printMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CTFSettingsDialog[] = {

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
       5,    0,   65,    2, 0x06 /* Public */,
       6,    1,   66,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   69,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    0,   71,    2, 0x08 /* Private */,
      10,    0,   72,    2, 0x08 /* Private */,
      11,    1,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void CTFSettingsDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CTFSettingsDialog *_t = static_cast<CTFSettingsDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Log((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->Finish((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->Start(); break;
        case 3: _t->RunError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->enableShellEdit(); break;
        case 5: _t->enableAmplitudeCorrection(); break;
        case 6: _t->generateShell(); break;
        case 7: _t->executeShell(); break;
        case 8: _t->printMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CTFSettingsDialog::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CTFSettingsDialog::Log)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CTFSettingsDialog::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CTFSettingsDialog::Finish)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CTFSettingsDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CTFSettingsDialog::Start)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CTFSettingsDialog::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CTFSettingsDialog::RunError)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject CTFSettingsDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CTFSettingsDialog.data,
      qt_meta_data_CTFSettingsDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CTFSettingsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CTFSettingsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CTFSettingsDialog.stringdata0))
        return static_cast<void*>(const_cast< CTFSettingsDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int CTFSettingsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void CTFSettingsDialog::Log(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CTFSettingsDialog::Finish(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CTFSettingsDialog::Start()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void CTFSettingsDialog::RunError(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
