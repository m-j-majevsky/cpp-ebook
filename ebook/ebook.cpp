#include <iostream>
#include <vector>

using namespace std;

class BookProgressTracker {
public:
    BookProgressTracker()
        : user_progress_(MAX_USER_COUNT_ + 1, 0)
        , book_progress_bar_(MAX_PAGE_COUNT_ + 1, 0) {
    }
    
    // Cохранить факт того, что пользователь под номером user дочитал книгу до страницы page. 
    void ReadProgress(unsigned int user, unsigned int page) {
        const unsigned int previous_user_progress = user_progress_[user];
        if (previous_user_progress == 0) {
            // Если ранее такой пользователь не встречался, нужно его добавить. 
            ++active_user_count_;
        }
        // Пользователь под номером user дочитал книгу до страницы page. 
        user_progress_[user] = page;
        // Также учитываем его прогресс между двумя событиями ReadProgress.
        for (unsigned int i = previous_user_progress + 1; i <= page; ++i) {
            ++book_progress_bar_[i];
        }
    }

    // Cообщить пользователю user, какая доля существующих пользователей, 
    // не считая его самого, прочитала меньшую часть книги, чем он. 
    double CheerUser(unsigned int user) const {
        const unsigned int user_progress = user_progress_[user];
        if (user_progress == 0) {
            // Если для данного пользователя пока не было ни одного события ReadProgress, 
            // то доля считается равной 0.
            return 0.;
        }
        if (active_user_count_ == 1) {
            // Если этот пользователь на данный момент единственный, 
            // то доля считается равной 1.
            return 1.;
        }
        return static_cast<double>(active_user_count_ - book_progress_bar_[user_progress]) / (active_user_count_ - 1);
    }

private:
    static constexpr int MAX_USER_COUNT_ = 100'000;
    static constexpr int MAX_PAGE_COUNT_ = 1'000;

    // В этом векторе по индексу n хранится номер страницы, 
    // до которой дочитал пользователь id, равным n. 
    vector<unsigned int> user_progress_;
    // А в этом - по индексу m хранится количество пользователей, 
    // дочитавших как минимум до страницы m.
    vector<unsigned int> book_progress_bar_;

    // Количество пользователей, для которых произошло по крайней мере одно событие ReadProgress.
    unsigned int active_user_count_{0};
};

int main() {
    BookProgressTracker db; 
    cout.precision(6);
    int query_count;
    cin >> query_count;
    for (int i = 0; i < query_count; ++i) {
        string command;
        cin >> command;
        unsigned int user;
        cin >> user;
        if (command == "READ"s) {
            unsigned int page;
            cin >> page;
            db.ReadProgress(user, page);
        } else {
            cout << db.CheerUser(user) << '\n';
        }
    }
    return 0;
}
