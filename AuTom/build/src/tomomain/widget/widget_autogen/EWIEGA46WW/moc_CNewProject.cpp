/****************************************************************************
** Meta object code from reading C++ file 'CNewProject.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/tomomain/widget/CNewProject.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CNewProject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CNewProjectWin_t {
    QByteArrayData data[13];
    char stringdata0[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CNewProjectWin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CNewProjectWin_t qt_meta_stringdata_CNewProjectWin = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CNewProjectWin"
QT_MOC_LITERAL(1, 15, 10), // "NewSuccess"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 8), // "filename"
QT_MOC_LITERAL(4, 36, 10), // "FindRawtlt"
QT_MOC_LITERAL(5, 47, 18), // "OpenExistedProject"
QT_MOC_LITERAL(6, 66, 13), // "CreateProject"
QT_MOC_LITERAL(7, 80, 16), // "QAbstractButton*"
QT_MOC_LITERAL(8, 97, 6), // "button"
QT_MOC_LITERAL(9, 104, 10), // "ChoicePath"
QT_MOC_LITERAL(10, 115, 13), // "ChoicePathMRC"
QT_MOC_LITERAL(11, 129, 15), // "FindRawtlt_Slot"
QT_MOC_LITERAL(12, 145, 21) // "OpenProjectBtnClicked"

    },
    "CNewProjectWin\0NewSuccess\0\0filename\0"
    "FindRawtlt\0OpenExistedProject\0"
    "CreateProject\0QAbstractButton*\0button\0"
    "ChoicePath\0ChoicePathMRC\0FindRawtlt_Slot\0"
    "OpenProjectBtnClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CNewProjectWin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    0,   57,    2, 0x06 /* Public */,
       5,    0,   58,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   59,    2, 0x08 /* Private */,
       9,    0,   62,    2, 0x08 /* Private */,
      10,    0,   63,    2, 0x08 /* Private */,
      11,    0,   64,    2, 0x08 /* Private */,
      12,    0,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CNewProjectWin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CNewProjectWin *_t = static_cast<CNewProjectWin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->NewSuccess((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->FindRawtlt(); break;
        case 2: _t->OpenExistedProject(); break;
        case 3: _t->CreateProject((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 4: _t->ChoicePath(); break;
        case 5: _t->ChoicePathMRC(); break;
        case 6: _t->FindRawtlt_Slot(); break;
        case 7: _t->OpenProjectBtnClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
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
            typedef void (CNewProjectWin::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CNewProjectWin::NewSuccess)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CNewProjectWin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CNewProjectWin::FindRawtlt)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CNewProjectWin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CNewProjectWin::OpenExistedProject)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject CNewProjectWin::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CNewProjectWin.data,
      qt_meta_data_CNewProjectWin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CNewProjectWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CNewProjectWin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CNewProjectWin.stringdata0))
        return static_cast<void*>(const_cast< CNewProjectWin*>(this));
    return QWidget::qt_metacast(_clname);
}

int CNewProjectWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CNewProjectWin::NewSuccess(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CNewProjectWin::FindRawtlt()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void CNewProjectWin::OpenExistedProject()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
