/****************************************************************************
** Meta object code from reading C++ file 'Project5.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../Project5.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Project5.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Project5_t {
    QByteArrayData data[17];
    char stringdata0[187];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Project5_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Project5_t qt_meta_stringdata_Project5 = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Project5"
QT_MOC_LITERAL(1, 9, 17), // "push_min_cvt_line"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 22), // "std::vector<CompdLine>"
QT_MOC_LITERAL(4, 51, 12), // "min_cvt_line"
QT_MOC_LITERAL(5, 64, 18), // "push_min_dist_line"
QT_MOC_LITERAL(6, 83, 9), // "CompdLine"
QT_MOC_LITERAL(7, 93, 13), // "min_dist_line"
QT_MOC_LITERAL(8, 107, 8), // "load_map"
QT_MOC_LITERAL(9, 116, 12), // "close_window"
QT_MOC_LITERAL(10, 129, 11), // "min_convert"
QT_MOC_LITERAL(11, 141, 8), // "min_dist"
QT_MOC_LITERAL(12, 150, 11), // "search_stop"
QT_MOC_LITERAL(13, 162, 15), // "recv_search_pos"
QT_MOC_LITERAL(14, 178, 1), // "x"
QT_MOC_LITERAL(15, 180, 1), // "y"
QT_MOC_LITERAL(16, 182, 4) // "kind"

    },
    "Project5\0push_min_cvt_line\0\0"
    "std::vector<CompdLine>\0min_cvt_line\0"
    "push_min_dist_line\0CompdLine\0min_dist_line\0"
    "load_map\0close_window\0min_convert\0"
    "min_dist\0search_stop\0recv_search_pos\0"
    "x\0y\0kind"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Project5[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       5,    1,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,
      10,    0,   62,    2, 0x08 /* Private */,
      11,    0,   63,    2, 0x08 /* Private */,
      12,    0,   64,    2, 0x08 /* Private */,
      13,    3,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   14,   15,   16,

       0        // eod
};

void Project5::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Project5 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->push_min_cvt_line((*reinterpret_cast< std::vector<CompdLine>(*)>(_a[1]))); break;
        case 1: _t->push_min_dist_line((*reinterpret_cast< CompdLine(*)>(_a[1]))); break;
        case 2: _t->load_map(); break;
        case 3: _t->close_window(); break;
        case 4: _t->min_convert(); break;
        case 5: _t->min_dist(); break;
        case 6: _t->search_stop(); break;
        case 7: _t->recv_search_pos((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Project5::*)(std::vector<CompdLine> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Project5::push_min_cvt_line)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Project5::*)(CompdLine );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Project5::push_min_dist_line)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Project5::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_Project5.data,
    qt_meta_data_Project5,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Project5::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Project5::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Project5.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Project5::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void Project5::push_min_cvt_line(std::vector<CompdLine> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Project5::push_min_dist_line(CompdLine _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
