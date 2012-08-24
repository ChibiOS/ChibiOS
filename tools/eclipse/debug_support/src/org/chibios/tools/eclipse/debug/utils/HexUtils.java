/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package org.chibios.tools.eclipse.debug.utils;

public class HexUtils {

  protected final static String[] hexChars = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"
  };

  static public boolean isHexChar(char c) {

    if (Character.isDigit(c) || (c >= 'a') && (c <= 'f') ||
        (c >= 'A') && (c <= 'F'))
      return true;
    return false;
  }

  static public boolean isHexString(String hstring) {
    int l = hstring.length();

    if ((l & 1) == 1)
      return false;

    for (int i = 0; i < l; i++)
      if (!isHexChar(hstring.charAt(i)))
        return false;
    return true;
  }

  static public byte[] hexStringToByteArray(String hstring) {

    if (!isHexString(hstring))
      throw new NumberFormatException("not a hex string");

    byte[] result = new byte[hstring.length() / 2];

    for (int i = 0; i < hstring.length(); i += 2) {
      String toParse = hstring.substring(i, i + 2);

      result[i / 2] = (byte)Integer.parseInt(toParse, 16);
    }
    return result;
  }

  static public byte hexStringToByte(String hstring) {

    if (hstring.length() != 2)
      throw new NumberFormatException("not a byte hex string");

    return (byte)Integer.parseInt(hstring, 16);
  }

  static public String byteArrayToHexString(byte[] data) {
    StringBuffer out = new StringBuffer(data.length * 2);

    for (int i = 0; i < data.length; i++) {
      out.append(hexChars[(data[i] >> 4) & 15]);
      out.append(hexChars[data[i] & 15]);
    }
    return out.toString();
  }

  static public String byte2HexString(int b) {

    return hexChars[(b >> 4) & 15] + hexChars[b & 15];
  }

  static public String word2HexString(int w) {

    return hexChars[(w >> 12) & 15]
         + hexChars[(w >> 8) & 15]
         + hexChars[(w >> 4) & 15]
         + hexChars[w & 15];
  }

  static public String dword2HexString(int w) {

    return hexChars[(w >> 28) & 15]
         + hexChars[(w >> 24) & 15]
         + hexChars[(w >> 20) & 15]
         + hexChars[(w >> 16) & 15]
         + hexChars[(w >> 12) & 15]
         + hexChars[(w >> 8) & 15]
         + hexChars[(w >> 4) & 15]
         + hexChars[w & 15];
  }
  
  static public long parseNumber(String s) {
    
    if (s.toLowerCase().startsWith("0x"))
      return Long.parseLong(s.substring(2), 16);
    return Long.parseLong(s);
  }
}

