#ifndef _BTLoop_H_
#define _BTLoop_H_

#include "base/BTBase.h"
namespace BehaviorTree
{
    class BTLoop : public BTBase
    {
    public:
        static const int LoopInfinite = -1;
        BTLoop(BTBase* parentNode, Precondition* precondition = nullptr)
            : BTBase(parentNode, precondition),
              loop_limit(LoopInfinite),
              loop_count(0),
              loop_index(0)
        {
            Init();
        }

        void SetLoopLimit(int limit)
        {
            loop_limit = limit;
        }
        void SetLoopCount(int count)
        {
            loop_count = count;
        }
        void SetLoopIndex(int index)
        {
            if(CheckIndex(index))
            {
                loop_index = index;
            }
        }

        virtual void Init() {}
        virtual bool DoEvaluate(void* object)
        {
            if(loop_limit != LoopInfinite && loop_count >= loop_limit)
            {
                return false;
            }

            if(CheckIndex(loop_index))
            {
                auto node = child[loop_index];

                if(node->Evaluate(object))
                {
                    return true;
                }
            }

            return false;
        }
        virtual RunningStatus DoExecute(void* object)
        {
            RunningStatus thisfinish = finish;

            if(CheckIndex(loop_index))
            {
                auto node = child[loop_index];
                thisfinish = node->Execute(object);

                if(thisfinish == finish)
                {
                    if(loop_limit == LoopInfinite)
                        thisfinish = running;
                    else
                    {
                        if(loop_count < loop_limit)
                            thisfinish = running;
                        else
                            thisfinish = finish;

                        loop_count++;
                    }
                }
            }

            if(thisfinish >= finish)
            {
                loop_count = 0;
            }

            return thisfinish;
        }
        virtual void DoTransition(void* object)
        {
            if(CheckIndex(loop_index))
            {
                auto node = child[loop_index];
                node->Transition(object);
            }

            loop_count = 0;
        }
    protected:
        int loop_limit;
        int loop_count;
        int loop_index;

    };
}

#endif // !_BTLoop_H_