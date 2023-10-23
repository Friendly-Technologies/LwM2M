#ifndef WPP_INST_SUBJECT_H_
#define WPP_INST_SUBJECT_H_

#include "Operation.h"
#include "InstObserver.h"

namespace wpp {

template <typename T>
class InstSubject {
public:
    /*
	 * Subscribers will be notified about the write, delete
	 * and execute of instance resources initiated by server.
	 */
	void subscribe(InstObserver<T> *observer) {
    if (!observer) return;
    if (std::find(_observers.begin(), _observers.end(), observer) == _observers.end()) 
        _observers.push_back(observer);
    }
	void unsubscribe(InstObserver<T> *observer) {
        _observers.erase(std::find(_observers.begin(), _observers.end(), observer));
    }

protected:
    /*
	 * Notify observers about operation
	 */
	void observerNotify(T &inst, const ResLink &resId, Operation::TYPE type) {
        for(InstObserver<T>* observer : _observers) {
            if (type == Operation::TYPE::READ) {
                observer->resourceRead(inst, resId);
            } else if (type == Operation::TYPE::WRITE) {
                observer->resourceWrite(inst, resId);
            } else if (type == Operation::TYPE::EXECUTE) {
                observer->resourceExecute(inst, resId);
            }
        }
    }

private:
    std::vector<InstObserver<T> *> _observers;
};

}

#endif //WPP_INST_SUBJECT_H_