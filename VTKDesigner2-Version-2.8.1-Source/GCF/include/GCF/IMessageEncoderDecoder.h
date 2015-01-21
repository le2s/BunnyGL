/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef I_MESSAGE_ENCODER_DECODER_H
#define I_MESSAGE_ENCODER_DECODER_H

#include <GCF/IContainer>

namespace GCF { class Message; }

class IMessageEncoderDecoder : virtual public IContainer
{
public:
    virtual bool canDecodeMessage(const QByteArray& ba) const = 0;
    virtual GCF::Message* decodeMessage(QByteArray& ba) const = 0;
    virtual QByteArray encodeMessage(const GCF::Message* msg) = 0;
    virtual bool isCloseSocketUponSendMessage() const = 0;
};
Q_DECLARE_INTERFACE(IMessageEncoderDecoder, "com.vcreatelogic.IMessageEncoderDecoder/1.0")

#endif

