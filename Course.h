#ifndef HW2_234218_W20_21_COURSE_H
#define HW2_234218_W20_21_COURSE_H

#include "HashTable.h"
#include "class.h"

class Course {
    int course_id;

    ///all classes of current course
    HashTable<Class>* course_classes;

    int num_of_classes;

public:
    explicit Course(int courseId, int numOfClasses = 0) :
    course_id(courseId), num_of_classes(numOfClasses) {
        this->course_classes = new HashTable<Class>(true);
    }

    virtual ~Course() {
        delete course_classes;
    }

    int getCourseId() const {
        return course_id;
    }

    void setCourseId(int courseId) {
        course_id = courseId;
    }

    HashTable<Class> *getCourseClasses() const {
        return course_classes;
    }

    void setCourseClasses(HashTable<Class> *courseClasses) {
        course_classes = courseClasses;
    }

    int getNumOfClasses() const {
        return num_of_classes;
    }

    void setNumOfClasses(int numOfClasses) {
        num_of_classes = numOfClasses;
    }

    int getTimeViewedOfClass(int classID){
        auto x = this->course_classes->find(classID);
        return nullptr == x ? -1 : x->getTime();
    }

    void addClass(int *classID){
        *classID = this->num_of_classes;
        auto *aClass = new Class(this->num_of_classes, this->course_id);
        this->course_classes->insertHash(aClass->getClassId(), aClass);
        this->num_of_classes++;
    }
};


#endif //HW2_234218_W20_21_COURSE_H
