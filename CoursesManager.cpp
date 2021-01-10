#include "CoursesManager.h"

CoursesManager::CoursesManager() {

}

CoursesManager::~CoursesManager() {

}

StatusType CoursesManager::AddCourse(int courseID) {
    return INVALID_INPUT;
}

StatusType CoursesManager::RemoveCourse(int courseID) {
    return INVALID_INPUT;
}

StatusType CoursesManager::AddClass(int courseID, int *classID) {
    if(this->numOfClasses < courseID + 1)
        return INVALID_INPUT;
    ///check in the hash table if this course exists...
}

StatusType CoursesManager::WatchClass(int courseID, int classID, int time) {
    return INVALID_INPUT;
}

StatusType
CoursesManager::TimeViewed(int courseID, int classID, int *timeViewed) {
    return INVALID_INPUT;
}

StatusType CoursesManager::GetIthWatchedClass(int i, int *courseID, int *classID) {
    return INVALID_INPUT;
}
