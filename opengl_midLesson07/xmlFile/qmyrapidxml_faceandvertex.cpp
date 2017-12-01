#include "qmyrapidxml_faceandvertex.h"

QMyRapidXML_FaceAndVertex::QMyRapidXML_FaceAndVertex()
{

}

char *QMyRapidXML_FaceAndVertex::readFile(const char *fileName)
{
    FILE*   pFile   =   fopen(fileName,"rb");
    if (pFile == 0)
    {
        return  false;
    }
    fseek(pFile,0,SEEK_END);
    size_t  length  =   ftell(pFile);
    fseek(pFile,0,SEEK_SET);
    char*   pText   =   new char[length + 1];
    fread(pText,1,length,pFile);
    pText[length]   =   0;
    fclose(pFile);
    return  pText;
}

bool QMyRapidXML_FaceAndVertex::load(const char *fileName)
{
    char*   pXML    =   readFile(fileName);
    if (pXML == 0)
    {
        return  false;
    }

    try
    {

        rapidxml::xml_document<>    doc;
        doc.parse<0>(pXML);
        rapidxml::xml_node<>*   pRoot   =   doc.first_node();
        rapidxml::xml_node<>*   pMesh   =   pRoot->first_node();
        rapidxml::xml_node<>*   pFaces  =   pMesh->first_node("faceIndex");
        rapidxml::xml_node<>*   vertexs =   pMesh->first_node("vertex");

        for (rapidxml::xml_node<>* face = pFaces->first_node() ; face != 0 ; face = face->next_sibling())
        {
            int     x,y,z;
            const char* strFace =   face->value();
            sscanf(strFace,"%d %d %d",&x,&y,&z);
            Face    faceIndex    =   {x,y,z};
            _arFaces.push_back(faceIndex);
        }

        for (rapidxml::xml_node<>* vert = vertexs->first_node() ; vert != 0 ; vert = vert->next_sibling())
        {
            Vertex  vVert;
            vVert.r =   1;
            vVert.g =   1;
            vVert.b =   1;
            vVert.a =   1;
            const char* strVert =   vert->value();
            sscanf(strVert,"%f %f %f %f %f %f %f %f",&vVert.x,&vVert.y,&vVert.z,&vVert.u,&vVert.v,&vVert.nx,&vVert.ny,&vVert.nz);

            _arVertex.push_back(vVert);
        }
    }
    catch(...)
    {
    }

    delete  []pXML;
    return  true;

}
