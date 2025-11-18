"""Script principal para iniciar a aplicação CampelMag."""

import sys

from PySide6.QtWidgets import QApplication

from .main_window import MainWindow


def main():
    """Iniciar aplicação."""
    app = QApplication(sys.argv)
    app.setApplicationName("CampelMag")
    app.setOrganizationName("CampelMag")

    window = MainWindow()
    window.show()

    sys.exit(app.exec())


if __name__ == "__main__":
    main()
