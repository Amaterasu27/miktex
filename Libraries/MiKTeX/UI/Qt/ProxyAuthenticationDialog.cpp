/* ProxyAuthenticationDialog.cpp:

   Copyright (C) 2008 Christian Schenk

   This file is part of the MiKTeX UI Library.

   The MiKTeX UI Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.

   The MiKTeX UI Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the MiKTeX UI Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */

#include "StdAfx.h"

#include "internal.h"
#include "ProxyAuthenticationDialog.h"

#include <miktex/UI/Qt/ErrorDialog>

/* _________________________________________________________________________

   ProxyAuthenticationDialog::ProxyAuthenticationDialog
   _________________________________________________________________________ */

ProxyAuthenticationDialog::ProxyAuthenticationDialog
(/*[in]*/ QWidget *		pParent)
  : QDialog (pParent)
{
  setupUi (this);
  QRegExp namePattern ("\\w+");
  QValidator * pValidator = new QRegExpValidator(namePattern, this);
  leName->setValidator (pValidator);
  leName->setText ("");
}

/* _________________________________________________________________________

   ProxyAuthenticationDialog::on_leName_textChanged
   _________________________________________________________________________ */

void
ProxyAuthenticationDialog::on_leName_textChanged
(/*[in]*/ const QString & newText)
{
  try
    {
      QPushButton * pOKButton = buttonBox->button(QDialogButtonBox::Ok);
      if (pOKButton == 0)
	{
	  UNEXPECTED_CONDITION
	    ("ProxyAuthenticationDialog::on_leName_textChanged");
	}
      pOKButton->setEnabled (newText.length() > 0);
    }
  catch (const MiKTeXException & e)
    {
      ErrorDialog::DoModal (this, e);
    }
  catch (const exception & e)
    {
      ErrorDialog::DoModal (this, e);
    }
}
