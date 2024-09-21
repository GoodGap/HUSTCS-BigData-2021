/****************************************************************************
** Meta object code from reading C++ file 'point_label.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../point_label.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'point_label.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_point_label_t {
    QByteArrayData data[18];
    char stringdata0[256];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_point_label_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_point_label_t qt_meta_stringdata_point_label = {
    {
QT_MOC_LITERAL(0, 0, 11), // "point_label"
QT_MOC_LITERAL(1, 12, 18), // "gen_point_area_sig"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 12), // "store_points"
QT_MOC_LITERAL(4, 45, 13), // "QList<QPoint>"
QT_MOC_LITERAL(5, 59, 6), // "points"
QT_MOC_LITERAL(6, 66, 11), // "store_lines"
QT_MOC_LITERAL(7, 78, 30), // "std::vector<std::vector<int> >"
QT_MOC_LITERAL(8, 109, 8), // "stop_mat"
QT_MOC_LITERAL(9, 118, 8), // "line_num"
QT_MOC_LITERAL(10, 127, 14), // "line_of_points"
QT_MOC_LITERAL(11, 142, 18), // "store_min_cvt_line"
QT_MOC_LITERAL(12, 161, 22), // "std::vector<CompdLine>"
QT_MOC_LITERAL(13, 184, 12), // "min_cvt_line"
QT_MOC_LITERAL(14, 197, 19), // "store_min_dist_line"
QT_MOC_LITERAL(15, 217, 9), // "CompdLine"
QT_MOC_LITERAL(16, 227, 13), // "min_dist_line"
QT_MOC_LITERAL(17, 241, 14) // "gen_point_area"

    },
    "point_label\0gen_point_area_sig\0\0"
    "store_points\0QList<QPoint>\0points\0"
    "store_lines\0std::vector<std::vector<int> >\0"
    "stop_mat\0line_num\0line_of_points\0"
    "store_min_cvt_line\0std::vector<CompdLine>\0"
    "min_cvt_line\0store_min_dist_line\0"
    "CompdLine\0min_dist_line\0gen_point_area"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_point_label[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   45,    2, 0x0a /* Public */,
       6,    3,   48,    2, 0x0a /* Public */,
      11,    1,   55,    2, 0x0a /* Public */,
      14,    1,   58,    2, 0x0a /* Public */,
      17,    0,   61,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7, QMetaType::Int, 0x80000000 | 7,    8,    9,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void,

       0        // eod
};

void point_label::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<point_label *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->gen_point_area_sig(); break;
        case 1: _t->store_points((*reinterpret_cast< QList<QPoint>(*)>(_a[1]))); break;
        case 2: _t->store_lines((*reinterpret_cast< std::vector<std::vector<int> >(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< std::vector<std::vector<int> >(*)>(_a[3]))); break;
        case 3: _t->store_min_cvt_line((*reinterpret_cast< std::vector<CompdLine>(*)>(_a[1]))); break;
        case 4: _t->store_min_dist_line((*reinterpret_cast< CompdLine(*)>(_a[1]))); break;
        case 5: _t->gen_point_area(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QPoint> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (point_label::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&point_label::gen_point_area_sig)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject point_label::staticMetaObject = { {
    QMetaObject::SuperData::link<QLabel::staticMetaObject>(),
    qt_meta_stringdata_point_label.data,
    qt_meta_data_point_label,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *point_label::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *point_label::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_point_label.stringdata0))
        return static_cast<void*>(this);
    return QLabel::qt_metacast(_clname);
}

int point_label::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void point_label::gen_point_area_sig()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
