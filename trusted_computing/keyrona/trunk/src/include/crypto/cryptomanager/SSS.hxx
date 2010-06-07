// Copyright (C) 2008, 2009 by Sirrix AG security technologies
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

/*!
 *  @file       SSS.hxx
 *  @brief      Shamir's Secret Sharing 
 *  @author     Marian Kuehnel ( m.kuehnel@sirrix.com)
 *  @date       Last modification at $Date: 2009-03-17$
 */
 
//------------------------------------------------------------------------
#include <Integer.hxx>
#include <ByteVector.hxx>
#include <vector>

//------------------------------------------------------------------------
    namespace algo
    {
    //------------------------------------------------------------------------
    /*
     * @brief Shamir Secret Sharing
     */
    class SSS
    {
        public:

            /*
             * @brief default constructor
             */
            SSS();

            /*
             * @brief default destructor
             */
            ~SSS();

            std::vector<utils::ByteVector> getX() { return myX; };
            std::vector<utils::ByteVector> getY() { return myY; };

            /*
             * @brief scheme initialisation( ByteVector version)
             *
             * @param all participants
             * @param participants needed for key reconstruction
             * @param key
             * @param modul
             */
            void initialisation( utils::ByteVector Participant,
                                    utils::ByteVector Access,
                                    utils::ByteVector Key,
                                    utils::ByteVector Modul);


            /*
             * @brief scheme reconstruction
             *
             * @param X shares
             * @param Y shares
             * @param modul
             * @return key
             */
            utils::ByteVector reconstruction( std::vector<utils::ByteVector> X,
                                                            std::vector<utils::ByteVector> Y,
                                                            utils::ByteVector Modul);


        private:

            /*
             * @brief assignment operator
             */
            SSS operator=(SSS &src);

            /*
             * @brief copy constructor
             */
            SSS(SSS &src);

            /*
             * @brief stores all x
             */
            std::vector<utils::ByteVector> myX;

            /*
             * @brief stores all y
             */
            std::vector<utils::ByteVector> myY;
        };
    }
