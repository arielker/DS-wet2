#ifndef HW2_234218_W20_21_CLASS_H
#define HW2_234218_W20_21_CLASS_H

///this class represents a Class (ironic right ?) in our "BOOM" system
class Class {
    int classID;
    int courseID;
    int time;

public:
    ///c'tor
    Class(int classId, int courseId, int time = 0) :
    classID(classId), courseID(courseId), time(time) {}

    ///d'tor
    virtual ~Class() = default;

    int getClassId() const {
        return classID;
    }

    void setClassId(int classId) {
        classID = classId;
    }

    int getCourseId() const {
        return courseID;
    }

    void setCourseId(int courseId) {
        courseID = courseId;
    }

    int getTime() const {
        return time;
    }

    void setTime(int _time) {
        Class::time += _time;
    }

    bool operator==(const Class &rhs) const {
        return classID == rhs.classID && courseID == rhs.courseID && time == rhs.time;
    }

    bool operator!=(const Class &rhs) const {
        return !(rhs == *this);
    }

    bool operator>(const Class &rhs) const{
        if (time == rhs.time){
            if (courseID == rhs.courseID) {
                return classID < rhs.classID;
            }
            return courseID < rhs.courseID;
        }
        return time > rhs.time;
    }

    bool operator<(const Class &rhs) const {
        return !(*this > rhs) && !(*this == rhs);
    }

    bool operator<=(const Class &rhs) const {
        return *this == rhs || *this < rhs;
    }

    bool operator>=(const Class &rhs) const {
        return *this == rhs || *this > rhs;
    }
};
#endif //HW2_234218_W20_21_CLASS_H
