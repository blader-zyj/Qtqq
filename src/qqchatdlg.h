#ifndef QTQQ_QQCHATDLG_H
#define QTQQ_QQCHATDLG_H

#include <QDateTime>
#include <QDialog>
#include <QHash>
#include <QScrollBar>
#include <QTcpSocket>
#include <QUuid>

#include "core/types.h"
#include "core/nameconvertor.h"
#include "core/qqmsglistener.h"
#include "core/qqmsgsender.h"
#include "msgbrowse.h"
#include "qqfacepanel.h"
#include "qqtextedit.h"

class QAction;
class QMenu;
class QShortcut;

class ImgSender;
class ImgLoader;
class QQChatLog;
class QQItem;

class QQChatDlg : public QWidget, public QQMsgListener
{
    Q_OBJECT
signals:
    void chatFinish(QQChatDlg *listener);
    void msgSended(QString to_uin, bool ok);

public:
    enum ChatDlgType {kGroup, kFriend};
    QQChatDlg(QString id, QString name, QWidget *parent = 0);
    virtual ~QQChatDlg();

public:
    void showMsg(ShareQQMsgPtr msg);
    void showOldMsg(QVector<ShareQQMsgPtr> msgs);
    QString id() const
    { return id_; }
    QString getUniqueId()
    {
        return QUuid::createUuid().toString();
    }

    void set_type(ChatDlgType type)
    { type_ = type; }
    ChatDlgType type() const
    { return type_; }
    virtual void updateSkin() = 0;


protected:
    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject * obj, QEvent * e);
    void jsonEncoding(QString &escasing);

protected:
    QString send_url_;
    MsgBrowse msgbrowse_;
    QQTextEdit te_input_;
    QHash<QString, FileInfo> id_file_hash_;

    QString id_;
    int msg_id_;
    QString name_;
    QTcpSocket fd_;
    QVector<QString> unconvert_ids_;
    QMenu *send_type_menu_;
    NameConvertor convertor_;

private slots:
    void openPathDialog(bool);
    void openQQFacePanel();
    void openChatLogWin();
    void sendMsg();
    void setFileInfo(QString unique_id, FileInfo file_info);
    void onMsgSendDone(bool ok, QString msg);
    void setSendByReturn(bool checked);
    void setSendByCtrlReturn(bool checked);

private:
    virtual QString converToJson(const QString &raw_msg) = 0;
    virtual ImgSender* getImgSender() const = 0;
    virtual QQChatLog *getChatlog() const;
    virtual ImgLoader* getImgLoader() const;
    virtual void getInfoById(QString id, QString &name, QString &avatar_path, bool &ok) const = 0;
    QString converToShow(const QString &converting_html);

private:    
    ImgSender *img_sender_;
    ImgLoader *img_loader_;
    QQFacePanel *qqface_panel_;
    QQMsgSender *msg_sender_;

    ChatDlgType type_;
    
    QAction *act_return_;
    QAction *act_ctrl_return_;
    
    bool send_by_return_;

    QShortcut *sc_close_win_;
};

#endif //QTQQ_QQCHATDLG_H