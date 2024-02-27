/****************************************************************************
** Meta object code from reading C++ file 'marktreeviewwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../src/tomomain/ImageViewWindow/widgets/marktreeviewwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'marktreeviewwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MarkTreeView_t {
    QByteArrayData data[9];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MarkTreeView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MarkTreeView_t qt_meta_stringdata_MarkTreeView = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MarkTreeView"
QT_MOC_LITERAL(1, 13, 19), // "currentIndexChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 7), // "current"
QT_MOC_LITERAL(4, 42, 8), // "previous"
QT_MOC_LITERAL(5, 51, 21), // "selectionIndexChanged"
QT_MOC_LITERAL(6, 73, 14), // "QItemSelection"
QT_MOC_LITERAL(7, 88, 8), // "selected"
QT_MOC_LITERAL(8, 97, 10) // "deselected"

    },
    "MarkTreeView\0currentIndexChanged\0\0"
    "current\0previous\0selectionIndexChanged\0"
    "QItemSelection\0selected\0deselected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MarkTreeView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,
       5,    2,   29,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QModelIndex, QMetaType::QModelIndex,    3,    4,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 6,    7,    8,

       0        // eod
};

void MarkTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MarkTreeView *_t = static_cast<MarkTreeView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->currentIndexChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 1: _t->selectionIndexChanged((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QItemSelection >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MarkTreeView::*_t)(const QModelIndex & , const QModelIndex & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MarkTreeView::currentIndexChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MarkTreeView::*_t)(const QItemSelection & , const QItemSelection & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MarkTreeView::selectionIndexChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject MarkTreeView::staticMetaObject = {
    { &QTreeView::staticMetaObject, qt_meta_stringdata_MarkTreeView.data,
      qt_meta_data_MarkTreeView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MarkTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MarkTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MarkTreeView.stringdata0))
        return static_cast<void*>(const_cast< MarkTreeView*>(this));
    return QTreeView::qt_metacast(_clname);
}

int MarkTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void MarkTreeView::currentIndexChanged(const QModelIndex & _t1, const QModelIndex & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MarkTreeView::selectionIndexChanged(const QItemSelection & _t1, const QItemSelection & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_TreeNodeInfoView_t {
    QByteArrayData data[1];
    char stringdata0[17];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TreeNodeInfoView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TreeNodeInfoView_t qt_meta_stringdata_TreeNodeInfoView = {
    {
QT_MOC_LITERAL(0, 0, 16) // "TreeNodeInfoView"

    },
    "TreeNodeInfoView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TreeNodeInfoView[] = {

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

void TreeNodeInfoView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject TreeNodeInfoView::staticMetaObject = {
    { &QTableView::staticMetaObject, qt_meta_stringdata_TreeNodeInfoView.data,
      qt_meta_data_TreeNodeInfoView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TreeNodeInfoView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TreeNodeInfoView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TreeNodeInfoView.stringdata0))
        return static_cast<void*>(const_cast< TreeNodeInfoView*>(this));
    return QTableView::qt_metacast(_clname);
}

int TreeNodeInfoView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_MarkManager_t {
    QByteArrayData data[11];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MarkManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MarkManager_t qt_meta_stringdata_MarkManager = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MarkManager"
QT_MOC_LITERAL(1, 12, 27), // "treeViewCurrentIndexChanged"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 7), // "current"
QT_MOC_LITERAL(4, 49, 8), // "previous"
QT_MOC_LITERAL(5, 58, 29), // "treeViewSelectionIndexChanged"
QT_MOC_LITERAL(6, 88, 14), // "QItemSelection"
QT_MOC_LITERAL(7, 103, 8), // "selected"
QT_MOC_LITERAL(8, 112, 10), // "deselected"
QT_MOC_LITERAL(9, 123, 15), // "treeViewClicked"
QT_MOC_LITERAL(10, 139, 5) // "index"

    },
    "MarkManager\0treeViewCurrentIndexChanged\0"
    "\0current\0previous\0treeViewSelectionIndexChanged\0"
    "QItemSelection\0selected\0deselected\0"
    "treeViewClicked\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MarkManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x08 /* Private */,
       5,    2,   34,    2, 0x08 /* Private */,
       9,    1,   39,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex, QMetaType::QModelIndex,    3,    4,
    QMetaType::Void, 0x80000000 | 6, QMetaType::QModelIndex,    7,    8,
    QMetaType::Void, QMetaType::QModelIndex,   10,

       0        // eod
};

void MarkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MarkManager *_t = static_cast<MarkManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->treeViewCurrentIndexChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 1: _t->treeViewSelectionIndexChanged((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 2: _t->treeViewClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QItemSelection >(); break;
            }
            break;
        }
    }
}

const QMetaObject MarkManager::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MarkManager.data,
      qt_meta_data_MarkManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MarkManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MarkManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MarkManager.stringdata0))
        return static_cast<void*>(const_cast< MarkManager*>(this));
    return QWidget::qt_metacast(_clname);
}

int MarkManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
