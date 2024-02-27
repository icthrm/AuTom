/****************************************************************************
** Meta object code from reading C++ file 'markcategorydialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../src/tomomain/ImageViewWindow/widgets/markcategorydialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'markcategorydialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MarkCategoryDialog_t {
    QByteArrayData data[6];
    char stringdata0[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MarkCategoryDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MarkCategoryDialog_t qt_meta_stringdata_MarkCategoryDialog = {
    {
QT_MOC_LITERAL(0, 0, 18), // "MarkCategoryDialog"
QT_MOC_LITERAL(1, 19, 14), // "resultReceived"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 4), // "name"
QT_MOC_LITERAL(4, 40, 5), // "color"
QT_MOC_LITERAL(5, 46, 17) // "createButtonClick"

    },
    "MarkCategoryDialog\0resultReceived\0\0"
    "name\0color\0createButtonClick"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MarkCategoryDialog[] = {

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
       5,    0,   29,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QColor,    3,    4,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void MarkCategoryDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MarkCategoryDialog *_t = static_cast<MarkCategoryDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->resultReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QColor(*)>(_a[2]))); break;
        case 1: _t->createButtonClick(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MarkCategoryDialog::*_t)(const QString & , const QColor & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MarkCategoryDialog::resultReceived)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MarkCategoryDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MarkCategoryDialog.data,
      qt_meta_data_MarkCategoryDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MarkCategoryDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MarkCategoryDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MarkCategoryDialog.stringdata0))
        return static_cast<void*>(const_cast< MarkCategoryDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int MarkCategoryDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void MarkCategoryDialog::resultReceived(const QString & _t1, const QColor & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
