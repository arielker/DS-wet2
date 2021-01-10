#ifndef HW2_234218_W20_21_COURSESMANAGER_H
#define HW2_234218_W20_21_COURSESMANAGER_H

#include "AVL.h"
#include "class.h"
#include "library2.h"
#include "Course.h"
#include "HashTable.h"

class CoursesManager {
    ///Ranked AVL tree of all classes in the system.
    ///NOTE: current amount of viewed classes in the system is the tree's size.
    AVL<int, Class*> *classes_tree;

    ///hash table of courses where for each course there is a hash table of  classes.
    HashTable<Course> *courses;

public:
    CoursesManager();
    ~CoursesManager();
    StatusType AddCourse(int courseID);
    StatusType RemoveCourse(int courseID);
    StatusType AddClass(int courseID, int* classID);
    StatusType WatchClass(int courseID, int classID, int time);
    StatusType TimeViewed(int courseID, int classID, int* timeViewed);
    StatusType GetIthWatchedClass(int i, int* courseID, int* classID);
};


#endif //HW2_234218_W20_21_COURSESMANAGER_H
