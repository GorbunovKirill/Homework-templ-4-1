#include <fstream>

class Printable
{
public:
    virtual ~Printable() = default;

    virtual std::string printAsHTML() const = 0; 
    // ������� ���������� ���������� (ISP): 
    // ��� ������, ����������� ���� ���������, ������ ������������� ��� ��� ������, ���� ���� ��������� �� ��� ����� �� �������������. 
    // ��� �������� � ������������ � �������� ISP.

    virtual std::string printAsText() const = 0; // ���������� ����������� �����������. 
    // ��������, ���� �����-��������� ������������ ������ ��� ���������� ������, ��� ��� ����� �������� ������������� ��������� ������.

    virtual std::string printAsJSON() const = 0; 
    // �����, ������, ������� �� �������� � JSON,��������� ������������� ���� �����, ��� �������� ������� ���������� ����������.
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
            // ������� ����������� ������ (LSP): 
//��� ������� �������� LSP, ��� ��� �������������, ��� ������������� ���������� �������� � 
// ������� ��� ������ �������, ���� ��� �� ������������� ��� ������������� �������. 
// �������� ������ �������� ��������� � ����� �����, ��� ������������ ������� �����.
            throw std::runtime_error("Invalid format!"); 
// ������������ ���������� ����� �������� ���������� LSP, ��� ��� ��� �������� ��������� ��������� ���������.
        }
        return "<html>" + data_ + "<html/>"; 
        // ������ � ����������� ���� <html/>; ������ ���� </html>.
    }

    std::string printAsText() const override
    {
        if (format_ != Format::kText) { // ���������� ����������� ����������� �� ������ LSP.
            throw std::runtime_error("Invalid format!"); 
            // ������������ ���������� ����� ����� �������� LSP.
        }
        return data_;
    }

    std::string printAsJSON() const override
    {
        if (format_ != Format::kJSON) { // ����� ��, ��� ������� �������� LSP.
            throw std::runtime_error("Invalid format!"); // ��������� LSP.
        }
        return "{ \"data\": \"" + data_ + "\"}";
    }

private:
    std::string data_;
    Format format_; // ������� ����������/���������� (OCP): 
    // ���� ����� �������� ����� ������, �������� XML,�������� �������������� ������������ ����� Data, 
    // ��� �������� OCP. ������ ����� ����� ������������ ������� "�������" ��� �������� �������� `Data` � ������ ���������.

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
