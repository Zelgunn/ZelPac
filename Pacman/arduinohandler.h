#ifndef ARDUINOHANDLER_H
#define ARDUINOHANDLER_H

#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <SFML/Window.hpp>

#include <QDebug>

#define BUF_LENGHT 32
#define LINE_SIZE 10

/**
 * @brief Classe gérant la connexion d'un Arduino Uno pour le contrôle du Pacman.
 * @author Valentin D.d.G.
 * @version 1.0
 * @details Un ArduinoHandler emet des signaux Gauche, Droite, Haut, Bas en fonction
 *          de la lecture (resp. '0', '1', '2' et '3') sur le port série où est branché l'Arduino Uno.
 *          N'émet aucun signal si aucun Arduino n'est branché.
 *          Si plusieurs Arduinos sont branchés, l'ArduinoHandler utilisera le premier
 *          disponible.
 */

class ArduinoHandler : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructeur de base ArduinoHandler
     * @details Initialise un ArduinoHandler. Cependant, il faut démarrer manuellement
     *          la connexion via la méthode start(). Le port série doit être ouvert avec
     *          un Baud rate de 9600.
     */
    ArduinoHandler();
    /**
     * @brief Connecte à l'Arduino Uno.
     * @details Connecte au port série du premier Arduino Uno trouvé. Si aucun Arduino Uno n'est disponible,
     *          cette fonction ne fait rien. La lecture du port est effectuée toutes les 20mscs.
     */
    void start();

protected slots:
    /**
     * @brief Boucle principale de lecture du port série.
     * @details Emet leftPressed en lisant '0'.
     *          \n Emet rightPressed en lisant '1'.
     *          \n Emet upPressed en lisant '2'.
     *          \n Emet downPressed en lisant '3'.
     */
    void mainloop();

signals:
    /**
     * @brief Emit lorsque l'Arduino Uno écrit '0' sur le port série.
     */
    void leftPressed();
    /**
     * @brief Emit lorsque l'Arduino Uno écrit '1' sur le port série.
     */
    void rightPressed();
    /**
     * @brief Emit lorsque l'Arduino Uno écrit '2' sur le port série.
     */
    void upPressed();
    /**
     * @brief Emit lorsque l'Arduino Uno écrit '3' sur le port série.
     */
    void downPressed();

private:
    /** @brief Timer de vérification du port série. */
    QTimer *m_timer;
    /** @brief Port série de communication. */
    QSerialPort *m_serialPort;
};

#endif // ARDUINOHANDLER_H
