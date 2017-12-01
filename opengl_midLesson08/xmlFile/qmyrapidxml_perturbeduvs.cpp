#include "qmyrapidxml_perturbeduvs.h"

QMyRapidXML_PerturbedUvs::QMyRapidXML_PerturbedUvs()
{

}

char *QMyRapidXML_PerturbedUvs::readFile(const char *fileName)
{
    FILE*   pFile   =   fopen(fileName,"rb");
    if (pFile == 0)
    {
        return  NULL;
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

bool QMyRapidXML_PerturbedUvs::load(const char *fileName)
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
        rapidxml::xml_node<>*   pFaces  =   pRoot->first_node("faces");
        rapidxml::xml_node<>*   vertexs =   pRoot->first_node("vertexs");
        rapidxml::xml_node<>*   normals =   pRoot->first_node("normals");
        rapidxml::xml_node<>*   tangents=   pRoot->first_node("tangents");
        rapidxml::xml_node<>*   uvws    =   pRoot->first_node("uvws");


        rapidxml::xml_node<>* normal    =   normals->first_node();
        rapidxml::xml_node<>* tangent   =   tangents->first_node();
        rapidxml::xml_node<>* uv        =   uvws->first_node();

        for (rapidxml::xml_node<>* vert = vertexs->first_node() ; vert != 0 ; vert = vert->next_sibling())
        {
            VertexBumpMap  vVert;
            const char* strVert =   vert->value();
            const char* strNor  =   normal->value();
            const char* strUV   =   uv->value();
            const char* strTan  =   tangent->value();
            sscanf(strVert, "%f %f %f", &vVert._pos.x,      &vVert._pos.y,      &vVert._pos.z);
            sscanf(strNor,  "%f %f %f", &vVert._normal.x,   &vVert._normal.y,   &vVert._normal.z);
            sscanf(strTan,  "%f %f %f", &vVert._tangent.x,  &vVert._tangent.y,  &vVert._tangent.z);
            sscanf(strUV,   "%f %f",    &vVert._uv.x,       &vVert._uv.y);

            _arVertexs.push_back(vVert);

            normal  =   normal->next_sibling();
            tangent =   tangent->next_sibling();
            uv      =   uv->next_sibling();
        }

        for (rapidxml::xml_node<>* face = pFaces->first_node() ; face != 0 ; face = face->next_sibling())
        {
            int     x,y,z;
            const char* strFace =   face->value();
            sscanf(strFace,"%d %d %d",&x,&y,&z);
            Face    faceIndex    =   {(short)x,(short)y,(short)z};
            _arFaces.push_back(faceIndex);
        }
    }
    catch(...)
    {
    }

    delete  []pXML;
    return  true;
}
