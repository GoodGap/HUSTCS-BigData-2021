/****************************************************************************
** Meta object code from reading C++ file 'file_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../file_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'file_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_file_widget_t {
    QByteArrayData data[13];
    char stringdata0[161];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_file_widget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_file_widget_t qt_meta_stringdata_file_widget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "file_widget"
QT_MOC_LITERAL(1, 12, 14), // "true_stop_file"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 13), // "QList<QPoint>"
QT_MOC_LITERAL(4, 42, 6), // "points"
QT_MOC_LITERAL(5, 49, 14), // "true_line_file"
QT_MOC_LITERAL(6, 64, 30), // "std::vector<std::vector<int> >"
QT_MOC_LITERAL(7, 95, 8), // "stop_mat"
QT_MOC_LITERAL(8, 104, 8), // "line_num"
QT_MOC_LITERAL(9, 113, 14), // "line_of_points"
QT_MOC_LITERAL(10, 128, 10), // "stop_input"
QT_MOC_LITERAL(11, 139, 10), // "line_input"
QT_MOC_LITERAL(12, 150, 10) // "check_file"

    },
    "file_widget\0true_stop_file\0\0QList<QPoint>\0"
    "points\0true_line_file\0"
    "std::vector<std::vector<int> >\0stop_mat\0"
    "line_num\0line_of_points\0stop_input\0"
    "line_input\0check_file"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_file_widget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       5,    3,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   49,    2, 0x0a /* Public */,
      11,    0,   50,    2, 0x0a /* Public */,
      12,    0,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int, 0x80000000 | 6,    7,    8,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void file_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<file_widget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->true_stop_file((*reinterpret_cast< QList<QPoint>(*)>(_a[1]))); break;
        case 1: _t->true_line_file((*reinterpret_cast< std::vector<std::vector<int> >(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< std::vector<std::vector<int> >(*)>(_a[3]))); break;
        case 2: _t->stop_input(); break;
        case 3: _t->line_input(); break;
        case 4: _t->check_file(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
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
            using _t = void (file_widget::*)(QList<QPoint> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&file_widget::true_stop_file)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (file_widget::*)(std::vector<std::vector<int>> , int , std::vector<std::vector<int>> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&file_widget::true_line_file)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject file_widget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_file_widget.data,
    qt_meta_data_file_widget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *file_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *file_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_file_widget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int file_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void file_widget::true_stop_file(QList<QPoint> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void file_widget::true_line_file(std::vector<std::vector<int>> _t1, int _t2, std::vector<std::vector<int>> _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
