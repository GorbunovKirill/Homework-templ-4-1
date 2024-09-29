#include <fstream>

class Printable
{
public:
    virtual ~Printable() = default;

    virtual std::string printAsHTML() const = 0; 
    // Принцип разделения интерфейса (ISP): 
    // Все классы, реализующие этот интерфейс, должны реализовывать все три метода, даже если некоторые из них могут не потребоваться. 
    // Это приводит к избыточности и нарушает ISP.

    virtual std::string printAsText() const = 0; // Аналогично предыдущему комментарию. 
    // Например, если класс-наследник предназначен только для текстового вывода, ему все равно придется реализовывать остальные методы.

    virtual std::string printAsJSON() const = 0; 
    // Снова, классы, которые не работают с JSON,вынуждены реализовывать этот метод, что нарушает принцип разделения интерфейса.
};

class Data : public Printable
{
public:
    enum class Format
    {
        kText,
        kHTML,
        kJSON
    };

    Data(std::string data, Format format)
        : data_(std::move(data)), format_(format) {}

    std::string printAsHTML() const override
    {
        if (format_ != Format::kHTML) { 
            // Принцип подстановки Лисков (LSP): 
//Это условие нарушает LSP, так как подразумевает, что использование подклассов приведет к 
// ошибкам при вызове методов, если они не предназначены для определенного формата. 
// Подкласс должен работать корректно в любом месте, где используется базовый класс.
            throw std::runtime_error("Invalid format!"); 
// Выбрасывание исключения также является нарушением LSP, так как это изменяет ожидаемое поведение программы.
        }
        return "<html>" + data_ + "<html/>"; 
        // Ошибка в закрывающем теге <html/>; должно быть </html>.
    }

    std::string printAsText() const override
    {
        if (format_ != Format::kText) { // Аналогично предыдущему комментарию по поводу LSP.
            throw std::runtime_error("Invalid format!"); 
            // Выбрасывание исключения здесь также нарушает LSP.
        }
        return data_;
    }

    std::string printAsJSON() const override
    {
        if (format_ != Format::kJSON) { // Опять же, это условие нарушает LSP.
            throw std::runtime_error("Invalid format!"); // Нарушение LSP.
        }
        return "{ \"data\": \"" + data_ + "\"}";
    }

private:
    std::string data_;
    Format format_; // Принцип открытости/закрытости (OCP): 
    // Если нужно добавить новый формат, например XML,придется модифицировать существующий класс Data, 
    // что нарушает OCP. Вместо этого можно использовать паттерн "Фабрика" для создания объектов `Data` с новыми форматами.

};

void saveTo(std::ofstream& file, const Printable& printable, Data::Format format)
{
    switch (format)
    {
    case Data::Format::kText:
        file << printable.printAsText();
        break;
    case Data::Format::kJSON:
        file << printable.printAsJSON();
        break;
    case Data::Format::kHTML:
        file << printable.printAsHTML();
        break;
    }
}

void saveToAsHTML(std::ofstream& file, const Printable& printable) {
    saveTo(file, printable, Data::Format::kHTML);
}

void saveToAsJSON(std::ofstream& file, const Printable& printable) {
    saveTo(file, printable, Data::Format::kJSON);
}

void saveToAsText(std::ofstream& file, const Printable& printable) {
    saveTo(file, printable, Data::Format::kText);
}
