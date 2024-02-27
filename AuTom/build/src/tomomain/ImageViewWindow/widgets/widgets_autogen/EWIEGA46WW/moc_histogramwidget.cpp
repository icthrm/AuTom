/****************************************************************************
** Meta object code from reading C++ file 'histogramwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../src/tomomain/ImageViewWindow/widgets/histogramwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'histogramwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Histogram_t {
    QByteArrayData data[7];
    char stringdata0[88];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Histogram_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Histogram_t qt_meta_stringdata_Histogram = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Histogram"
QT_MOC_LITERAL(1, 10, 15), // "minValueChanged"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 5), // "value"
QT_MOC_LITERAL(4, 33, 15), // "maxValueChanged"
QT_MOC_LITERAL(5, 49, 18), // "setLeftCursorValue"
QT_MOC_LITERAL(6, 68, 19) // "setRightCursorValue"

    },
    "Histogram\0minValueChanged\0\0value\0"
    "maxValueChanged\0setLeftCursorValue\0"
    "setRightCursorValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Histogram[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   40,    2, 0x0a /* Public */,
       6,    1,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void Histogram::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Histogram *_t = static_cast<Histogram *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->minValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->maxValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setLeftCursorValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setRightCursorValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Histogram::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Histogram::minValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Histogram::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Histogram::maxValueChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject Histogram::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Histogram.data,
      qt_meta_data_Histogram,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Histogram::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Histogram::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Histogram.stringdata0))
        return static_cast<void*>(const_cast< Histogram*>(this));
    return QWidget::qt_metacast(_clname);
}

int Histogram::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Histogram::minValueChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Histogram::maxValueChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_ImagePreviewWidget_t {
    QByteArrayData data[1];
    char stringdata0[19];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImagePreviewWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImagePreviewWidget_t qt_meta_stringdata_ImagePreviewWidget = {
    {
QT_MOC_LITERAL(0, 0, 18) // "ImagePreviewWidget"

    },
    "ImagePreviewWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImagePreviewWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void ImagePreviewWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ImagePreviewWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ImagePreviewWidget.data,
      qt_meta_data_ImagePreviewWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ImagePreviewWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImagePreviewWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ImagePreviewWidget.stringdata0))
        return static_cast<void*>(const_cast< ImagePreviewWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ImagePreviewWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_EqualizationControlWidget_t {
    QByteArrayData data[3];
    char stringdata0[40];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EqualizationControlWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EqualizationControlWidget_t qt_meta_stringdata_EqualizationControlWidget = {
    {
QT_MOC_LITERAL(0, 0, 25), // "EqualizationControlWidget"
QT_MOC_LITERAL(1, 26, 12), // "valueChanged"
QT_MOC_LITERAL(2, 39, 0) // ""

    },
    "EqualizationControlWidget\0valueChanged\0"
    ""
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EqualizationControlWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void EqualizationControlWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EqualizationControlWidget *_t = static_cast<EqualizationControlWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (EqualizationControlWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EqualizationControlWidget::valueChanged)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject EqualizationControlWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_EqualizationControlWidget.data,
      qt_meta_data_EqualizationControlWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *EqualizationControlWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EqualizationControlWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_EqualizationControlWidget.stringdata0))
        return static_cast<void*>(const_cast< EqualizationControlWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int EqualizationControlWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void EqualizationControlWidget::valueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_FilterControlWidget_t {
    QByteArrayData data[3];
    char stringdata0[28];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FilterControlWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FilterControlWidget_t qt_meta_stringdata_FilterControlWidget = {
    {
QT_MOC_LITERAL(0, 0, 19), // "FilterControlWidget"
QT_MOC_LITERAL(1, 20, 6), // "filter"
QT_MOC_LITERAL(2, 27, 0) // ""

    },
    "FilterControlWidget\0filter\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FilterControlWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void FilterControlWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FilterControlWidget *_t = static_cast<FilterControlWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->filter(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FilterControlWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FilterControlWidget::filter)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject FilterControlWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FilterControlWidget.data,
      qt_meta_data_FilterControlWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FilterControlWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FilterControlWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FilterControlWidget.stringdata0))
        return static_cast<void*>(const_cast< FilterControlWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FilterControlWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void FilterControlWidget::filter()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_BrightnessContrastControlWidget_t {
    QByteArrayData data[3];
    char stringdata0[46];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BrightnessContrastControlWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BrightnessContrastControlWidget_t qt_meta_stringdata_BrightnessContrastControlWidget = {
    {
QT_MOC_LITERAL(0, 0, 31), // "BrightnessContrastControlWidget"
QT_MOC_LITERAL(1, 32, 12), // "valueChanged"
QT_MOC_LITERAL(2, 45, 0) // ""

    },
    "BrightnessContrastControlWidget\0"
    "valueChanged\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BrightnessContrastControlWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void BrightnessContrastControlWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BrightnessContrastControlWidget *_t = static_cast<BrightnessContrastControlWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BrightnessContrastControlWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BrightnessContrastControlWidget::valueChanged)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject BrightnessContrastControlWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_BrightnessContrastControlWidget.data,
      qt_meta_data_BrightnessContrastControlWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BrightnessContrastControlWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BrightnessContrastControlWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BrightnessContrastControlWidget.stringdata0))
        return static_cast<void*>(const_cast< BrightnessContrastControlWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int BrightnessContrastControlWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void BrightnessContrastControlWidget::valueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_HistogramWidget_t {
    QByteArrayData data[11];
    char stringdata0[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HistogramWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HistogramWidget_t qt_meta_stringdata_HistogramWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "HistogramWidget"
QT_MOC_LITERAL(1, 16, 15), // "minValueChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 5), // "value"
QT_MOC_LITERAL(4, 39, 15), // "maxValueChanged"
QT_MOC_LITERAL(5, 55, 6), // "commit"
QT_MOC_LITERAL(6, 62, 5), // "image"
QT_MOC_LITERAL(7, 68, 17), // "onMinValueChanged"
QT_MOC_LITERAL(8, 86, 17), // "onMaxValueChanged"
QT_MOC_LITERAL(9, 104, 18), // "resetOriginalImage"
QT_MOC_LITERAL(10, 123, 15) // "updateDataModel"

    },
    "HistogramWidget\0minValueChanged\0\0value\0"
    "maxValueChanged\0commit\0image\0"
    "onMinValueChanged\0onMaxValueChanged\0"
    "resetOriginalImage\0updateDataModel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HistogramWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    1,   52,    2, 0x06 /* Public */,
       5,    1,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   58,    2, 0x08 /* Private */,
       8,    1,   61,    2, 0x08 /* Private */,
       9,    0,   64,    2, 0x08 /* Private */,
      10,    0,   65,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QImage,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HistogramWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HistogramWidget *_t = static_cast<HistogramWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->minValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->maxValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->commit((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        case 3: _t->onMinValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onMaxValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->resetOriginalImage(); break;
        case 6: _t->updateDataModel(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HistogramWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HistogramWidget::minValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (HistogramWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HistogramWidget::maxValueChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (HistogramWidget::*_t)(const QImage & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HistogramWidget::commit)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject HistogramWidget::staticMetaObject = {
    { &AbstractSliceViewPlugin::staticMetaObject, qt_meta_stringdata_HistogramWidget.data,
      qt_meta_data_HistogramWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *HistogramWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HistogramWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_HistogramWidget.stringdata0))
        return static_cast<void*>(const_cast< HistogramWidget*>(this));
    return AbstractSliceViewPlugin::qt_metacast(_clname);
}

int HistogramWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractSliceViewPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void HistogramWidget::minValueChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HistogramWidget::maxValueChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HistogramWidget::commit(const QImage & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
