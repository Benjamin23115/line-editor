#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

class TextEditor
{
public:
    TextEditor(const string &filename = "") : currentLine(0), bufferDirty(false), currentFilename(filename)
    {
        if (!filename.empty())
        {
            ifstream file(filename);
            if (file.is_open())
            {
                string line;
                while (getline(file, line))
                {
                    buffer.push_back(line);
                }
                file.close();
            }
            else
            {
                DisplayError("Error opening file.");
            }
        }
        else
        {
            buffer.push_back(""); // Start with an empty line
        }
    }

    // Command Descriptions
    void W(const string &filename = "");
    void J(int lineNumber);
    void I(const string &text = "");
    void L(int startLine = 0, int endLine = -1);
    void D(int startLine = -1, int endLine = -1);
    void A(const string &text = "");
    bool Q();

private:
    vector<string> buffer;
    int currentLine;
    bool bufferDirty;
    string currentFilename;
    // Helper function to display error messages

    void DisplayError(const string &message);
    // Helper function to save the buffer to a file

    bool SaveBufferToFile(const string &filename);
    // Helper function to list lines between start and end line numbers

    void ListLines(int startLine, int endLine);
};

// Saves the buffer to the specified file or the current filename.
void TextEditor::W(const string &filename)
{
    if (!filename.empty())
    {

        if (SaveBufferToFile(filename))
        {
            currentFilename = filename;
            bufferDirty = false;
        }
        else
        {
            DisplayError("Error writing to file.");
        }
    }
    else if (currentFilename.empty())
    {
        DisplayError("A filename must be given.");
    }
    else
    {
        if (SaveBufferToFile(currentFilename))
        {
            bufferDirty = false;
        }
        else
        {
            DisplayError("Error writing to file.");
        }
    }
}

// Moves the current line to the specified line number.
void TextEditor::J(int lineNumber)
{

    if (lineNumber == 0)
    {
        currentLine = 0;
    }
    else if (lineNumber == -1)
    {
        currentLine = buffer.size() - 1;
    }
    else if (lineNumber > 0 && lineNumber <= buffer.size())
    {
        currentLine = lineNumber - 1;
    }
    else
    {
        DisplayError("Invalid line number.");
    }
}
// Inserts text at the beginning of the current line.

void TextEditor::I(const string &text)
{
    if (currentLine < buffer.size())
    {
        buffer[currentLine].insert(buffer[currentLine].begin(), text.begin(), text.end());
        bufferDirty = true;
    }
    else
    {
        DisplayError("Invalid current line.");
    }
}
// Lists lines based on given start and end line numbers.

void TextEditor::L(int startLine, int endLine)
{
    if (startLine == 0)
    {
        startLine = 1;
    }

    if (startLine > 0 && startLine <= buffer.size())
    {
        if (endLine == -1)
        {
            // Only one line number given; show that line.
            cout << buffer[startLine - 1] << endl;
        }
        else if (endLine > 0 && endLine <= buffer.size() && endLine >= startLine)
        {
            // Two line numbers given; list lines between the numbers.
            for (int i = startLine - 1; i < endLine; i++)
            {
                cout << buffer[i] << endl;
            }
        }
        else
        {
            DisplayError("Invalid line numbers.");
        }
    }
    else
    {
        DisplayError("Invalid line numbers.");
    }
}
// Deletes lines based on given start and end line numbers.

void TextEditor::D(int startLine, int endLine)
{
    if (startLine == -1)
    {
        startLine = currentLine + 1;
        endLine = currentLine + 1;
    }

    if (startLine > 0 && startLine <= buffer.size() && endLine > 0 && endLine <= buffer.size() && startLine <= endLine)
    {
        buffer.erase(buffer.begin() + startLine - 1, buffer.begin() + endLine);
        bufferDirty = true;
    }
    else
    {
        DisplayError("Invalid line numbers.");
    }
}
// Appends text after the current line or inserts an empty line.

void TextEditor::A(const string &text)
{
    if (currentLine < buffer.size())
    {
        if (text.empty())
        {
            buffer.insert(buffer.begin() + currentLine + 1, "");
        }
        else
        {
            buffer.insert(buffer.begin() + currentLine + 1, text);
        }
        currentLine++;
        bufferDirty = true;
    }
    else
    {
        DisplayError("Invalid current line.");
    }
}
// Checks if the buffer has been modified and offers to save changes then closes the executable.

bool TextEditor::Q()
{
    if (bufferDirty)
    {
        string response;
        cout << "Buffer has been modified. Save changes before quitting? (Y/N): ";
        cin >> response;
        if (response == "Y" || response == "y")
        {
            W(currentFilename);
        }
    }
    return true;
}

void TextEditor::DisplayError(const string &message)
{
    cerr << "Error: " << message << endl;
}

bool TextEditor::SaveBufferToFile(const string &filename)
{
    ofstream file(filename);
    if (file.is_open())
    {
        for (const string &line : buffer)
        {
            file << line << "\n";
        }
        file.close();
        return true;
    }
    return false;
}

void TextEditor::ListLines(int startLine, int endLine)
{
    for (int i = startLine - 1; i < endLine; i++)
    {
        cout << buffer[i] << endl;
    }
}

int main(int argc, char *argv[])
{
    TextEditor editor;

    if (argc > 1)
    {
        editor = TextEditor(argv[1]);
    }
    string command;

    while (true)
    {
        cout << "> ";
        cin >> command;

        if (command == "W")
        {
            string filename;
            cin >> filename;
            editor.W(filename);
        }
        else if (command == "J")
        {
            int lineNumber;
            cin >> lineNumber;
            editor.J(lineNumber);
        }
        else if (command == "I")
        {
            string text;
            cin.ignore(); // Consume the newline character
            getline(cin, text);
            editor.I(text);
        }
        else if (command == "L")
        {
            int startLine, endLine;
            cin >> startLine >> endLine;
            editor.L(startLine, endLine);
        }
        else if (command == "D")
        {
            int startLine, endLine;
            cin >> startLine >> endLine;
            editor.D(startLine, endLine);
        }
        else if (command == "A")
        { // Added "A" command
            string text;
            cin.ignore(); // Consume the newline character
            getline(cin, text);
            editor.A(text);
        }
        else if (command == "Q")
        {
            if (editor.Q())
            {
                break;
            }
        }
        else
        {
            cout << "Invalid command." << endl;
        }
    }

    return 0;
}
