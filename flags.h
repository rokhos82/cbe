namespace BE {
    // Status codes
    const unsigned long valNormal = 0x0;
    const unsigned long valFighter = 0x1;
    const unsigned long valReserve = 0x2;
    const unsigned long valSurprise = 0x4;
    const unsigned long valClocked = 0x8;
    const unsigned long valMissile = 0x10;
    const unsigned long valMovement = 0x20;
    const unsigned long valNoBeam = 0x40;
    const unsigned long valNoTorp = 0x80;
    const unsigned long valCripple = 0x100;
    const unsigned long valFlak = 0x200;
    const unsigned long valMulti = 0x400;
    const unsigned long valAmmo = 0x800;
    const unsigned long valStasis = 0x1000;
    const unsigned long valPen = 0x2000;
    const unsigned long valSuicide = 0x4000;

    // Special attribute flags
    const unsigned long saDis = 0x1;
    const unsigned long saHeat = 0x2;
    const unsigned long saMeson = 0x4;
    const unsigned long saVibro = 0x8;
    const unsigned long saMulti = 0x10;
    const unsigned long saCrack = 0x20;
    const unsigned long saBp = 0x40;
    const unsigned long saPen = 0x80;
    const unsigned long saSpecial = 0x100;
}