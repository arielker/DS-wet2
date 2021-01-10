#include "library2.h"
#include "CoursesManager.h"

void *Init(){
    auto CM = new CoursesManager();
    return CM ? CM : nullptr;
}

StatusType AddCourse(void *DS, int courseID) {
    if(nullptr == DS || courseID <= 0){
        return INVALID_INPUT;
    }
    return ((CoursesManager *)DS)->AddCourse(courseID);
}

StatusType RemoveCourse(void *DS, int courseID) {
    if(nullptr == DS || courseID <= 0){
        return INVALID_INPUT;
    }
    return ((CoursesManager*)DS)->RemoveCourse(courseID);
}

StatusType AddClass(void *DS, int courseID, int *classID) {
    if(nullptr == DS || courseID <= 0 || nullptr == classID){
        return INVALID_INPUT;
    }
    return ((CoursesManager *)DS)->AddClass(courseID, classID);
}

StatusType WatchClass(void *DS, int courseID, int classID, int time) {
    if(nullptr == DS || courseID <= 0 || classID < 0 || time <= 0){
        return INVALID_INPUT;
    }
    return ((CoursesManager *)DS)->WatchClass(courseID, classID, time);
}

StatusType TimeViewed(void *DS, int courseID, int classID, int *timeViewed) {
    if(nullptr == DS || courseID <= 0 || classID < 0 || nullptr == timeViewed){
        return INVALID_INPUT;
    }
    return ((CoursesManager *)DS)->TimeViewed(courseID, classID, timeViewed);

}

StatusType GetIthWatchedClass(void *DS, int i, int *courseID, int *classID) {
    if(nullptr == DS || i <= 0 || nullptr == classID || nullptr == courseID){
        return INVALID_INPUT;
    }
    return ((CoursesManager *)DS)->GetIthWatchedClass(i, courseID, classID);
}

void Quit(void **DS) {
    if(nullptr != DS && nullptr != *DS) {
        delete *((CoursesManager **) DS);
    }
    *DS = nullptr;
}
