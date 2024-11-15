#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <optional>

class Book {
private:
    std::string title;
    std::string author;
    std::string isbn;
    bool isAvailable;

public:
    Book(const std::string& title, const std::string& author, const std::string& isbn)
        : title(title), author(author), isbn(isbn), isAvailable(true) {}

    void markAsLoaned() { isAvailable = false; }
    void markAsAvailable() { isAvailable = true; }

    bool isBookAvailable() const { return isAvailable; }

    const std::string& getTitle() const { return title; }
    const std::string& getAuthor() const { return author; }
    const std::string& getIsbn() const { return isbn; }

    void display() const {
        std::cout << "Название: " << title << ", Автор: " << author
                  << ", ISBN: " << isbn
                  << ", Статус: " << (isAvailable ? "Доступна" : "Выдана") << "\n";
    }
};

class Reader {
private:
    int id;
    std::string name;
    std::string email;

public:
    Reader(int id, const std::string& name, const std::string& email)
        : id(id), name(name), email(email) {}

    int getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getEmail() const { return email; }

    void display() const {
        std::cout << "Читатель: " << name << " (ID: " << id << ", Почта: " << email << ")\n";
    }
};

class Loan {
private:
    Book& book;
    Reader& reader;
    time_t loanDate;
    std::optional<time_t> returnDate;

public:
    Loan(Book& book, Reader& reader)
        : book(book), reader(reader), loanDate(std::time(nullptr)), returnDate(std::nullopt) {}

    void completeLoan() {
        returnDate = std::time(nullptr);
        book.markAsAvailable();
    }

    void display() const {
        std::cout << "Запись выдачи - Книга: " << book.getTitle()
                  << ", Читатель: " << reader.getName()
                  << ", Дата выдачи: " << std::ctime(&loanDate)
                  << "Дата возврата: " << (returnDate ? std::ctime(&returnDate.value()) : "Не возвращена\n");
    }
};

class Librarian {
private:
    std::vector<Book> books;
    std::vector<Reader> readers;

public:
    void addBook(const Book& book) {
        books.push_back(book);
        std::cout << "Книга '" << book.getTitle() << "' добавлена.\n";
    }

    void removeBook(const std::string& isbn) {
        auto it = std::remove_if(books.begin(), books.end(),
                                 [&isbn](const Book& b) { return b.getIsbn() == isbn && b.isBookAvailable(); });

        if (it != books.end()) {
            books.erase(it, books.end());
            std::cout << "Книга успешно удалена.\n";
        } else {
            std::cout << "Книга не найдена или в данный момент выдана.\n";
        }
    }

    void addReader(const Reader& reader) {
        readers.push_back(reader);
        std::cout << "Читатель '" << reader.getName() << "' добавлен.\n";
    }

    void removeReader(int id) {
        auto it = std::remove_if(readers.begin(), readers.end(),
                                 [id](const Reader& r) { return r.getId() == id; });

        if (it != readers.end()) {
            readers.erase(it, readers.end());
            std::cout << "Читатель успешно удалён.\n";
        } else {
            std::cout << "Читатель не найден.\n";
        }
    }

    const std::vector<Reader>& getReaders() const {
        return readers;
    }

    Book* findBookByTitle(const std::string& title) {
        auto it = std::find_if(books.begin(), books.end(),
                               [&title](const Book& b) { return b.getTitle() == title; });
        return (it != books.end()) ? &(*it) : nullptr;
    }

    void displayBooks() const {
        std::cout << "Книги в библиотеке:\n";
        for (const auto& book : books) {
            book.display();
        }
    }

    void displayReaders() const {
        std::cout << "Зарегистрированные читатели:\n";
        for (const auto& reader : readers) {
            reader.display();
        }
    }
};

int main() {
    Librarian librarian;

    // Добавление книг
    librarian.addBook({"Искусство войны", "Сунь Цзы", "КН001"});
    librarian.addBook({"Размышления", "Марк Аврелий", "КН002"});

    // Добавление читателей
    librarian.addReader({1, "Иван Иванов", "ivanov@example.com"});
    librarian.addReader({2, "Мария Петрова", "petrova@example.com"});

    // Показ книг и читателей
    librarian.displayBooks();
    librarian.displayReaders();

    // Выдача книги
    Book* book = librarian.findBookByTitle("Искусство войны");
    if (book && book->isBookAvailable()) {
        int readerId = 1; // ID читателя
        Reader* reader = nullptr;

        for (const auto& r : librarian.getReaders()) {
            if (r.getId() == readerId) {
                reader = const_cast<Reader*>(&r);
                break;
            }
        }

        if (reader) {
            Loan loan(*book, *reader);
            book->markAsLoaned();

            std::cout << "Выдача создана:\n";
            loan.display();
        } else {
            std::cout << "Читатель с ID " << readerId << " не найден.\n";
        }
    } else {
        std::cout << "Книга недоступна для выдачи.\n";
    }

    // Обновление списка книг
    librarian.displayBooks();

    return 0;
}


