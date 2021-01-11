#include "CoursesManager.h"

CoursesManager::CoursesManager() {
    this->classes_tree = new AVL<int, Class*>();
    this->courses = new HashTable<Course>(true);
}

CoursesManager::~CoursesManager() {
    delete this->classes_tree;
    delete this->courses;
}

StatusType CoursesManager::AddCourse(int courseID) {
    if(nullptr != this->courses->find(courseID)){
        return FAILURE;
    }
    Course* course = new Course(courseID);
    this->courses->insertHash(courseID, course);
    return SUCCESS;
}

StatusType CoursesManager::RemoveCourse(int courseID) {
    auto course = this->courses->find(courseID);
    if (nullptr == course){
        return FAILURE;
    }
    for (int i = 0; i < course->getNumOfClasses(); ++i) {
        auto aClass = new Class(i, courseID, course->getTimeViewedOfClass(i));
        this->classes_tree->deleteNode(aClass, 0);
        delete aClass;
    }
    this->courses->deleteFromHash(courseID);
    return SUCCESS;
}

StatusType CoursesManager::AddClass(int courseID, int *classID) {
    auto course = this->courses->find(courseID);
    if(nullptr == course){
        return FAILURE;
    }
    course->addClass(classID);
    return SUCCESS;
}

StatusType CoursesManager::WatchClass(int courseID, int classID, int time) {
    auto course = this->courses->find(courseID);
    if(nullptr == course){
        return FAILURE;
    }
    int old_time = course->getTimeViewedOfClass(classID);
    if(course->getNumOfClasses() < classID + 1 || -1 == old_time){
        return INVALID_INPUT;
    }
    auto aClass = new Class(classID, courseID, old_time);
    this->classes_tree->deleteNode(aClass, 0);
    aClass->setTime(time);
    this->classes_tree->insert(0, aClass);
    course->getCourseClasses()->find(classID)->setTime(time);
    return SUCCESS;
}

StatusType CoursesManager::TimeViewed(int courseID, int classID, int *timeViewed) {
    auto course = this->courses->find(courseID);
    if(nullptr == course){
        return FAILURE;
    }
    int t = course->getTimeViewedOfClass(classID);
    if(course->getNumOfClasses() < classID + 1 || -1 == t){
        return INVALID_INPUT;
    }
    *timeViewed = t;
    return SUCCESS;
}

StatusType CoursesManager::GetIthWatchedClass(int i, int *courseID, int *classID) {
    if(this->classes_tree->getSize() < i){
        return FAILURE;
    }
    auto node = this->classes_tree->GetIthWatchedClass(i);
    *courseID = node->getCourseId();
    *classID = node->getClassId();
    return SUCCESS;
}
