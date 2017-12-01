#include "qmyrapidxml.h"

QMyRapidXML::QMyRapidXML()
{

}

char *QMyRapidXML::readFile(const char *fileName)
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

bool QMyRapidXML::loadScene(const char *fileScene)
{
    char*   pXML    =   readFile(fileScene);
    if (pXML == 0)
    {
        return  false;
    }

    try
    {

        rapidxml::xml_document<>    doc;
        doc.parse<0>(pXML);
        rapidxml::xml_node<>*   pRoot   =   doc.first_node();
        rapidxml::xml_node<>*   node    =   pRoot->first_node();

        for ( ; node != 0 ; node = node->next_sibling())
        {
            rapidxml::xml_attribute<>*  attrName    =   node->first_attribute("name");
            rapidxml::xml_attribute<>*  attrFile    =   node->first_attribute("file");
            rapidxml::xml_attribute<>*  attrPos     =   node->first_attribute("pos");
            rapidxml::xml_attribute<>*  attrRot     =   node->first_attribute("rot");
            rapidxml::xml_attribute<>*  attrScale   =   node->first_attribute("scale");

            if (_map.find(attrFile->value()) ==_map.end())
            {
                Model   model;
                load(attrFile->value(),model);
                _map[attrFile->value()] =   model;
            }
            Node    node;
            node._fileName  =   attrFile->value();
            sscanf(attrPos->value(),"%f %f %f",&node._pos.x,&node._pos.y,&node._pos.z);
            sscanf(attrRot->value(),"%f %f %f",&node._rot.x,&node._rot.y,&node._rot.z);
            sscanf(attrScale->value(),"%f %f %f",&node._scale.x,&node._scale.y,&node._scale.z);
            _arNode.push_back(node);
        }
    }
    catch(...)
    {
    }

    delete  []pXML;
    return  true;
}

bool QMyRapidXML::load(const char *fileName, QMyRapidXML::Model &mode)
{
    std::map<std::string,Model>::iterator itr = _map.find(fileName);
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
            mode._arFaces.push_back(faceIndex);
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

            mode._arVertex.push_back(vVert);
        }
    }
    catch(...)
    {
    }

    delete  []pXML;
    return  true;

}
