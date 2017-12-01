#pragma once

#include "rapidxml.hpp"
#include "CELLMath.hpp"
#include "CELL3RDCamera.hpp"
#include "CELLProgramLibrary.hpp"


namespace   CELL
{
    struct  VertexBumpMap
    {
        float3  _pos;
        float3  _normal;
        float2  _uv;
        float3  _tangent;
    };

    struct  Face
    {
        short   x,y,z;
    };

    class   CELLModel
    {
    public:
        CELLOpenGL& _device;
        std::vector<VertexBumpMap> _arVertexs;
        std::vector<Face>   _arFaces;

    public:

        CELLModel(CELLOpenGL& device)
            :_device(device)
        {
        }

        /**
        *   读文件函数，返回文件的内容，需要自己释放内存
        */

        virtual char*   readFile(const char* fileName)
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

        /**
        *   加载模型文件函数
        */

        virtual bool    load(const char* fileName)
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

        /**
        *   绘制函数
        */
        virtual void    onRender(Program_BasicLight& shader,CELL3RDCamera& camera)
        {
            static  float   angle = 0;
          
            matrix4 matRot(1);
            matRot.rotateY(angle);
            matrix4 matTrans;
            matTrans.translate(0,5,0);
            matrix4 matScale(1);
            matScale.scale(0.3f,0.3f,0.3f);

            matrix4 matModel    =   matTrans * matRot * matScale;

            angle   +=  1.0f;

            matrix4 MVP     =   camera._matProj * camera._matView * matModel ;
            matrix4 mv      =   camera._matView * matModel ;
            matrix4 mvIT    =   mv._inverse();

            float3 vMsLightDir(0,0,1);

            _device.setUniformMatrix4fv(shader.MVPMatrix,1,false,MVP.data());
            _device.setUniformMatrix4fv(shader.ModelViewIT,1,false,mvIT.data());

            _device.setUniform3fv(shader.LightDirection,1,&vMsLightDir.x);



            VertexBumpMap*  vert    =   &_arVertexs.front();
            _device.attributePointer(shader.inVertex,   3,  GL_FLOAT,   GL_FALSE, sizeof(VertexBumpMap), vert);
            _device.attributePointer(shader.inNormal,   3,  GL_FLOAT,   GL_FALSE, sizeof(VertexBumpMap), &vert[0]._normal);
            _device.attributePointer(shader.inTexCoord, 2,  GL_FLOAT,   GL_FALSE, sizeof(VertexBumpMap), &vert[0]._uv);
           
            _device.drawElements(GL_TRIANGLES,_arFaces.size() * 3,GL_UNSIGNED_SHORT,&_arFaces.front());
        }

    };
}