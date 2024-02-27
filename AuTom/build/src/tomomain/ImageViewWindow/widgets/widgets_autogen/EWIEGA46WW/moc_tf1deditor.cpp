/****************************************************************************
** Meta object code from reading C++ file 'tf1deditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../src/tomomain/ImageViewWindow/widgets/tf1deditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tf1deditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TF1DEditor_t {
    QByteArrayData data[17];
    char stringdata0[244];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TF1DEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TF1DEditor_t qt_meta_stringdata_TF1DEditor = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TF1DEditor"
QT_MOC_LITERAL(1, 11, 11), // "TF1DChanged"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 21), // "toggleInteractionMode"
QT_MOC_LITERAL(4, 46, 2), // "on"
QT_MOC_LITERAL(5, 49, 21), // "resetTransferFunction"
QT_MOC_LITERAL(6, 71, 20), // "loadTransferFunction"
QT_MOC_LITERAL(7, 92, 6), // "TFName"
QT_MOC_LITERAL(8, 99, 20), // "saveTransferFunction"
QT_MOC_LITERAL(9, 120, 22), // "updateTransferFunction"
QT_MOC_LITERAL(10, 143, 16), // "thresholdChanged"
QT_MOC_LITERAL(11, 160, 3), // "min"
QT_MOC_LITERAL(12, 164, 3), // "max"
QT_MOC_LITERAL(13, 168, 25), // "lowerThresholdSpinChanged"
QT_MOC_LITERAL(14, 194, 5), // "value"
QT_MOC_LITERAL(15, 200, 25), // "upperThresholdSpinChanged"
QT_MOC_LITERAL(16, 226, 17) // "toggleInteraction"

    },
    "TF1DEditor\0TF1DChanged\0\0toggleInteractionMode\0"
    "on\0resetTransferFunction\0loadTransferFunction\0"
    "TFName\0saveTransferFunction\0"
    "updateTransferFunction\0thresholdChanged\0"
    "min\0max\0lowerThresholdSpinChanged\0"
    "value\0upperThresholdSpinChanged\0"
    "toggleInteraction"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TF1DEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    1,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   73,    2, 0x0a /* Public */,
       6,    0,   74,    2, 0x0a /* Public */,
       6,    1,   75,    2, 0x0a /* Public */,
       8,    0,   78,    2, 0x0a /* Public */,
       9,    0,   79,    2, 0x0a /* Public */,
      10,    2,   80,    2, 0x0a /* Public */,
      13,    1,   85,    2, 0x0a /* Public */,
      15,    1,   88,    2, 0x0a /* Public */,
      16,    1,   91,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,   11,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Bool,    4,

       0        // eod
};

void TF1DEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TF1DEditor *_t = static_cast<TF1DEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->TF1DChanged(); break;
        case 1: _t->toggleInteractionMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->resetTransferFunction(); break;
        case 3: _t->loadTransferFunction(); break;
        case 4: _t->loadTransferFunction((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->saveTransferFunction(); break;
        case 6: _t->updateTransferFunction(); break;
        case 7: _t->thresholdChanged((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 8: _t->lowerThresholdSpinChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->upperThresholdSpinChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->toggleInteraction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TF1DEditor::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TF1DEditor::TF1DChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TF1DEditor::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TF1DEditor::toggleInteractionMode)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject TF1DEditor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TF1DEditor.data,
      qt_meta_data_TF1DEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TF1DEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TF1DEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TF1DEditor.stringdata0))
        return static_cast<void*>(const_cast< TF1DEditor*>(this));
    return QWidget::qt_metacast(_clname);
}

int TF1DEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void TF1DEditor::TF1DChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void TF1DEditor::toggleInteractionMode(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
