////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TankData.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class Mesh;

/**
* IDs for the tank physics elements
*/
struct TankPhysicsIDs
{
    int Body = 0;
    int Gun = 0;
    int Hinge = 0;
    int P1 = 0;
    int P2 = 0;
    int P3 = 0;
    int P4 = 0;
};

/**
* Holds individual pieces of the overall tank graphics mesh
*/
struct TankMesh
{
    /**
    * Constructor
    */
    TankMesh(Mesh& body,
             Mesh& gun,
             Mesh& p1,
             Mesh& p2,
             Mesh& p3,
             Mesh& p4) :
        Body(body),
        Gun(gun),
        P1(p1),
        P2(p2),
        P3(p3),
        P4(p4)
    {
    }

    Mesh& Body; ///< Main body of the tank
    Mesh& Gun;  ///< Gun on top of the tank
    Mesh& P1;   ///< First breakable piece
    Mesh& P2;   ///< Second breakable piece
    Mesh& P3;   ///< Third breakable piece
    Mesh& P4;   ///< Forth breakable piece
};