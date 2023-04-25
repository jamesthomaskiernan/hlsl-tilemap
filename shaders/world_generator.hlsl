#include "noise.hlsl"

// This buffer holds an int for each file; it will be read by the tilemap.hlsl pixel shader
RWStructuredBuffer<int> tiles : register (u0);

static int2 tilemapDimensions = int2(128, 128);

int PositionToIndex(int2 position)
{
    return (position.y * tilemapDimensions.x) + position.x;
}

// Uses noise to return a base type given a position
int GetTileType(int2 position)
{
    float noiseVal = SimplexNoise(float3(position.x * .1f, position.y * .1f, 1));
    
    // Add another layer of noise for added complexity
    noiseVal += SimplexNoise(float3(position.x * .01f, position.y * .01f, 1));

    // Water
    if (noiseVal < -.2f) {return 181;}

    // Grass
    if (noiseVal < .7f) {return 240;}

    // Trees
    else {return 190;}
}

// This function has a thread run for each tile position. It will calculate
// which tile goes in that position, and place that info in the tiles buffer.
// It first gets the base type for each tile adjacent, and then updates
// the tile type depending on adjacent values.
[numthreads(8, 8, 1)]
void Compute (uint3 id : SV_DispatchThreadID)
{   
    int2 tilePos = int2((int)id.x, (int)id.y);
    int index = PositionToIndex(id.xy);
    
    // Find base type of each position
    int thisType = GetTileType(tilePos);
    int left = GetTileType(int2(tilePos.x - 1, tilePos.y));
    int right = GetTileType(int2(tilePos.x + 1, tilePos.y));
    int up = GetTileType(int2(tilePos.x, tilePos.y + 1));
    int down = GetTileType(int2(tilePos.x, tilePos.y - 1));
    int up_left = GetTileType(int2(tilePos.x - 1, tilePos.y + 1));
    int up_right = GetTileType(int2(tilePos.x + 1, tilePos.y + 1));
    int down_left = GetTileType(int2(tilePos.x - 1, tilePos.y - 1));
    int down_right = GetTileType(int2(tilePos.x + 1, tilePos.y - 1));

    tiles[index] = thisType;

    // Each if statement below updates the texture depending on adjacent values

    // Water
    if (thisType == 181)
    {
        // First check up, down, left, right
        uint mask = 0;

        if (left != 181) {mask += 1;}
        if (right != 181) {mask += 2;}
        if (up != 181) {mask += 4;}
        if (down != 181) {mask += 8;}

        // This array simply maps a mask value to tile value
        static uint map[16];

        map[1] = 180; // only left
        map[2] = 182; // only right
        map[4] = 193; // only up
        map[8] = 169; // only down

        map[5] = 192; // up and left
        map[6] = 194; // up and right
        map[9] = 168; // down and left
        map[10] = 170; // down and right

        map[12] = 173; // up and down
        map[3] = 172; // left and right
        map[7] = 184; // left, right, up
        map[11] = 185; // left, right, down
        map[13] = 196; // up, down, left
        map[14] = 197; // up, down, right

        map[15] = 195; // up, down, left, right
        
        // If there are dirt tiles left, right, up, or down
        if (mask != 0) {tiles[index] = map[mask];}
        
        // Otherwise, do corner tiles
        else
        {
            if (down_right != 181) {mask += 1;}
            if (down_left != 181) {mask += 2;}
            if (up_right != 181) {mask += 4;}
            if (up_left != 181) {mask += 8;}

            map[1] = 156; // down_right corner
            map[2] = 157; // down_left corner
            map[4] = 144; // up_right corner
            map[8] = 145; // up_left corner

            map[12] = 149; // top corners
            map[3] = 160;  // down corners
            map[5] = 148;  // right corners
            map[10] = 161; // left corners

            if (mask != 0) {tiles[index] = map[mask];}
        }
    }

    // Grass
    else if (thisType == 240)
    {
        float noiseVal = SimplexNoise(float3(tilePos.x * .1f + .5f, tilePos.y * .1f + .5f, 1));

        // Heavy grass
        if (noiseVal > .4f) {tiles[index] = 242;}

        // Light grass
        else if (noiseVal > 0) {tiles[index] = 241;}


        noiseVal = SimplexNoise(float3(tilePos.x, tilePos.y, 1));

        uint flowerFrequency = 20;

        // Blue flowers
        if ((uint)(noiseVal.x * flowerFrequency) % 5 == 3) {tiles[index] = 229;}
        // Purple flowers
        if ((uint)(noiseVal.x * flowerFrequency) % 10 == 3) {tiles[index] = 217;}
        // Orange flower
        if ((uint)(noiseVal.x * flowerFrequency) % 15 == 3) {tiles[index] = 205;}
    }

    // Trees
    else if (thisType == 190)
    {
        // First check up, down, left, right
        uint mask = 0;

        if (left != 190) {mask += 1;}
        if (right != 190) {mask += 2;}
        if (up != 190) {mask += 4;}
        if (down != 190) {mask += 8;}

        // This array simply maps a mask value to tile value
        static uint map[16];

        map[0] = 190; // surrounded by tree

        map[1] = 189; // only left
        map[2] = 191; // only right
        map[4] = 202; // only up
        map[8] = 178; // only down

        map[5] = 201; // up and left
        map[6] = 203; // up and right
        map[9] = 177; // down and left
        map[10] = 179; // down and right

        map[7] = 154; // left, right, up
        map[11] = 166; // left, right, down
        map[13] = 153; // up, down, left
        map[14] = 165; // up, down, right

        map[15] = 167; // up, down, left, right
        
        // If there are tree tiles left, right, up, or down
        tiles[index] = map[mask] == 0 ? 190 : map[mask];
    }
}