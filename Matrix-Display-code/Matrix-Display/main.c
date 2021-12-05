/*
  * ShiftRegister.c
  *
  * Created: 6/6/2013 14:57:28
  * (c) Muchiri John
  *
  * Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.

  * Neither the name of the copyright holders nor the names of
  contributors may be used to endorse or promote products derived
  from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. See the GNU General Public License for more details.
*/

#include <avr/io.h>
#include "effects.h"
/************************************************************************/
/* animate images/text													*/
/************************************************************************/
char numbers[] = "012345678";
uint8_t index_num = 0;

static void Animate()
{
	Clear();
	//scroll long left
	//ScrollLongLeft("MAKERHACKS", 10);
	HoldDisplayBuffer(StringBytes("MAKERHACKS", LEFT), 50);
	ScrollLeft(StringBytes("MAKEHACKS", LEFT), 10);
	//blink
	Blinker(StringBytes("MAKE", LEFT), 10, 4);
	//scroll right
	ScrollRight(StringBytes("MAKE", LEFT), 10);
	//drop up
	Drop(StringBytes("HACK", CENTRE), 10, up);
	//hold display
	HoldDisplayBuffer(StringBytes("HACK", CENTRE), 50);
	//fall up
	Fall(StringBytes("HACK", CENTRE), 10, up);
	//drop down
	Drop(StringBytes("CODE", RIGHT), 10, down);
	//hold display
	HoldDisplayBuffer(StringBytes("CODE", RIGHT), 50);
	//fall down
	Fall(StringBytes("CODE", RIGHT), 10, down);
	//type word in
	TypeIn(StringBytes("JOIN", LEFT), 10);
	//hold display
	HoldDisplayBuffer(StringBytes("JOIN", LEFT), 50);
	//delete left
	DeleteLeft(StringBytes("JOIN", LEFT),  10);
	//split display
	Rain(StringBytes("SHARE", CENTRE), 100);
	//hold display
	HoldDisplayBuffer(StringBytes("SHARE", CENTRE), 50);
	//delete left
	DeleteRight(StringBytes("SHARE", CENTRE),  10);
	//scroll long left
	ScrollLongLeft("MAKERHACKS by MUCHIRIJOHN", 10);
}
/************************************************************************/
/* main function                                                        */
/************************************************************************/
int main(void)
{
	init();
    while(1)
    {
		Animate();
    }
}