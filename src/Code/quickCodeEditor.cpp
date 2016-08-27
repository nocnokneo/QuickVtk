#include "quickCodeEditor.hpp"
#include "quickCodeHighlighter.hpp"
#include "quickUtilIO.hpp"
#include "quickCompiler.hpp"

#include <QTextOption>
#include <QApplication>
#include <QClipboard>
#include <iostream>

namespace quick {

    namespace Code {

        Editor* Editor::instance = nullptr;
        Qml::Register::Controller<Editor> Editor::Register;

        Editor::Editor() {
            if (instance) {
                throw std::runtime_error("instance already existing");
            }

            this->m_highlighter     = nullptr;
            this->m_selectionStart  = 0;
            this->m_selectionEnd    = 0;
            this->m_editorCursor    = 0;
            this->m_line            = 0;
            this->m_column          = 0;
        }

        auto Editor::Create() -> void {
            instance = new Editor();
        }

        auto Editor::GetInstance() -> Editor* {
            return instance;
        }

        auto Editor::setText(const QString& text) -> void {
            this->m_document->textDocument()->setPlainText(text);
        }

        auto Editor::getText() -> QString {
            return this->m_document->textDocument()->toPlainText();
        }

        auto Editor::setDocument(QQuickTextDocument* document) -> void {
            this->m_document = document;
            this->m_document->textDocument()->setIndentWidth(2);

            auto options = this->m_document->textDocument()->defaultTextOption();
            options.setTabStop(20);
            options.setFlags(QTextOption::ShowTabsAndSpaces | QTextOption::ShowLineAndParagraphSeparators);
            this->m_document->textDocument()->setDefaultTextOption(options);

            emit this->documentChanged();

            this->m_highlighter = new Highlighter(this);
        }

        auto Editor::setLine(int line) -> void {
            if (this->m_line != line) {
                this->m_line = line;
                emit this->lineChanged();
            }
        }

        auto Editor::getLine() -> int {
            return this->m_line;
        }

        auto Editor::setColumn(int column) -> void {
            if (this->m_column != column) {
                this->m_column = column;
                emit this->columnChanged();
            }
        }

        auto Editor::getColumn() -> int {
            return this->m_column;
        }

        auto Editor::setSelectionStart(int pos) -> void {
            this->m_selectionStart = pos;

            emit this->selectionStartChanged();
        }

        auto Editor::setSelectionEnd(int pos) -> void {
            this->m_selectionEnd = pos;

            emit this->selectionEndChanged();

            auto cursor = getCurrentCursor();
            cursor.setPosition(pos);
            this->setLine(cursor.blockNumber());
            this->setColumn(cursor.columnNumber());
        }

        auto Editor::setFilePath(const QString& filePath) -> void {
            this->m_filePath = filePath;
            emit this->filePathChanged();
        }

        auto Editor::getFilePath() -> QString {
            return this->m_filePath;
        }

        auto Editor::getDocument() -> QQuickTextDocument* {
            return this->m_document;
        }

        auto Editor::getSelectionStart() -> int {
            return this->m_selectionStart;
        }

        auto Editor::getSelectionEnd() -> int {
            return this->m_selectionEnd;
        }

        auto Editor::getCurrentCursor() -> QTextCursor {
            auto c = QTextCursor(this->m_document->textDocument());

            c.setPosition(this->m_selectionEnd, QTextCursor::MoveAnchor);
            c.setPosition(this->m_selectionStart, QTextCursor::KeepAnchor);

            return c;
        }

        auto Editor::setEditorCursor(int position) -> void {
            this->m_editorCursor = position;

            emit this->editorCursorChanged();
        }

        auto Editor::getEditorCursor() -> int {
            return this->m_editorCursor;
        }

        auto Editor::setFontSize(int fontSize) -> void {
            this->m_fontSize = fontSize;
            emit this->fontSizeChanged();
        }

        auto Editor::getFontSize() -> int {
            return this->m_fontSize;
        }

        auto Editor::setModified(bool modified) -> void {
            if (this->m_modified != modified) {
                this->m_modified = modified;
                emit this->modifiedChanged();
            }
        }

        auto Editor::getModified() -> bool {
            return this->m_modified;
        }

        auto Editor::onKeyPressed(int key, int modifiers, const QString& string) -> bool {
            if (modifiers == Qt::ControlModifier) {
                if (key == Qt::Key_Plus) {
                    this->setFontSize(this->m_fontSize + 2);
                    return true;
                }

                if (key == Qt::Key_Minus) {
                    this->setFontSize(this->m_fontSize - 2);
                    return true;
                }

                if (key == Qt::Key_S) {
                    this->saveFile();
                    return true;
                }
                if (key == Qt::Key_R) {
                    this->run();
                    return true;
                }
                if (key == Qt::Key_O) {
                    this->openFile();
                    return true;
                }
                if (key == Qt::Key_N) {
                    this->newFile();
                    return true;
                }
                if (key == Qt::Key_V) {
                    auto text = QApplication::clipboard()->text();

                    if (!text.isEmpty())
                    {
                        auto cursor = this->getCurrentCursor();
                        cursor.beginEditBlock();
                        cursor.insertText(text);
                        cursor.endEditBlock();

                        this->setModified(true);
                    }
                    return true;
                }
            }

            if (QRegularExpression("(\\w+)|(\\W+)").match(string).hasMatch())
            {
                this->setModified(true);
            }

            return false;
        }

        void Editor::run() {
            Compiler::instance->setSource(this->getText());
        }

        void Editor::saveFile() {
            if (this->m_filePath.length() > 0 && Util::IO::FileExists(this->m_filePath)) {
                Util::IO::Write::TextToFile(this->getText(), this->m_filePath);
                this->setModified(false);
            } else {
                this->m_filePath = Util::IO::FromDialog::SelectSaveFileUrl("*.qml");
                emit this->filePathChanged();

                if (Util::IO::FileExists(this->m_filePath)) {
                    Util::IO::Write::TextToFile(this->getText(), this->m_filePath);
                    this->setModified(false);
                }
            }
        }

        void Editor::openFile() {
            auto newFilePath = Util::IO::FromDialog::SelectOpenFileUrl("*.qml");

            if (Util::IO::FileExists(newFilePath)) {
                this->m_filePath = newFilePath;
                emit this->filePathChanged();

                this->setText(Util::IO::Read::TextFromUrl(this->m_filePath));
                this->setModified(false);

                this->run();
            }
        }

        void Editor::newFile() {
            this->m_filePath = "";
            emit this->filePathChanged();

            this->setText("");
            this->setModified(false);
        }

        Editor::~Editor() {
            delete this->m_highlighter;
        }
    }
}