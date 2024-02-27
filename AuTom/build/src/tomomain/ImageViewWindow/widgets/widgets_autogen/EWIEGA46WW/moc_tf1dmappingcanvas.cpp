/****************************************************************************
** Meta object code from reading C++ file 'tf1dmappingcanvas.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../src/tomomain/ImageViewWindow/widgets/tf1dmappingcanvas.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tf1dmappingcanvas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TF1DMappingCanvas_t {
    QByteArrayData data[15];
    char stringdata0[205];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TF1DMappingCanvas_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TF1DMappingCanvas_t qt_meta_stringdata_TF1DMappingCanvas = {
    {
QT_MOC_LITERAL(0, 0, 17), // "TF1DMappingCanvas"
QT_MOC_LITERAL(1, 18, 7), // "changed"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 20), // "loadTransferFunction"
QT_MOC_LITERAL(4, 48, 20), // "saveTransferFunction"
QT_MOC_LITERAL(5, 69, 21), // "resetTransferFunction"
QT_MOC_LITERAL(6, 91, 17), // "toggleInteraction"
QT_MOC_LITERAL(7, 109, 2), // "on"
QT_MOC_LITERAL(8, 112, 14), // "splitMergeKeys"
QT_MOC_LITERAL(9, 127, 7), // "zeroKey"
QT_MOC_LITERAL(10, 135, 9), // "deleteKey"
QT_MOC_LITERAL(11, 145, 17), // "resetTransferFunc"
QT_MOC_LITERAL(12, 163, 18), // "changeCurrentColor"
QT_MOC_LITERAL(13, 182, 1), // "c"
QT_MOC_LITERAL(14, 184, 20) // "toggleClipThresholds"

    },
    "TF1DMappingCanvas\0changed\0\0"
    "loadTransferFunction\0saveTransferFunction\0"
    "resetTransferFunction\0toggleInteraction\0"
    "on\0splitMergeKeys\0zeroKey\0deleteKey\0"
    "resetTransferFunc\0changeCurrentColor\0"
    "c\0toggleClipThresholds"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TF1DMappingCanvas[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    0,   76,    2, 0x06 /* Public */,
       5,    0,   77,    2, 0x06 /* Public */,
       6,    1,   78,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   81,    2, 0x0a /* Public */,
       9,    0,   82,    2, 0x0a /* Public */,
      10,    0,   83,    2, 0x0a /* Public */,
      11,    0,   84,    2, 0x0a /* Public */,
      12,    0,   85,    2, 0x0a /* Public */,
      12,    1,   86,    2, 0x0a /* Public */,
      14,    0,   89,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor,   13,
    QMetaType::Void,

       0        // eod
};

void TF1DMappingCanvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TF1DMappingCanvas *_t = static_cast<TF1DMappingCanvas *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changed(); break;
        case 1: _t->loadTransferFunction(); break;
        case 2: _t->saveTransferFunction(); break;
        case 3: _t->resetTransferFunction(); break;
        case 4: _t->toggleInteraction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->splitMergeKeys(); break;
        case 6: _t->zeroKey(); break;
        case 7: _t->deleteKey(); break;
        case 8: _t->resetTransferFunc(); break;
        case 9: _t->changeCurrentColor(); break;
        case 10: _t->changeCurrentColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 11: _t->toggleClipThresholds(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TF1DMappingCanvas::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TF1DMappingCanvas::changed)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TF1DMappingCanvas::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TF1DMappingCanvas::loadTransferFunction)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TF1DMappingCanvas::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TF1DMappingCanvas::saveTransferFunction)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (TF1DMappingCanvas::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TF1DMappingCanvas::resetTransferFunction)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (TF1DMappingCanvas::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TF1DMappingCanvas::toggleInteraction)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject TF1DMappingCanvas::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TF1DMappingCanvas.data,
      qt_meta_data_TF1DMappingCanvas,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TF1DMappingCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TF1DMappingCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TF1DMappingCanvas.stringdata0))
        return static_cast<void*>(const_cast< TF1DMappingCanvas*>(this));
    return QWidget::qt_metacast(_clname);
}

int TF1DMappingCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void TF1DMappingCanvas::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void TF1DMappingCanvas::loadTransferFunction()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void TF1DMappingCanvas::saveTransferFunction()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void TF1DMappingCanvas::resetTransferFunction()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void TF1DMappingCanvas::toggleInteraction(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
