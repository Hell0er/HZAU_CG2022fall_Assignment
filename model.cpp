#include "model.h"

/*
 * @方法 读取模型
 * @参数 filePath obj文件的文件名
 * @参数 indexes 顶点的索引信息
 * @参数 vertexCount 顶点个数
 * @参数 indexCount  索引个数
 *
 * @返回 结构体数组 保存了obj模型的所有顶点位置信息、顶点纹理坐标、顶点法线信息
 */
VertexData *LoadObjModel(const char *filePath, unsigned int **indexes,
                         int &vertexCount, int &indexCount) {
  //获取文本内容
  char *fileContent = LoadFileContent(filePath);
  if (fileContent != nullptr) {
    //坐标
    /*struct VertexInfo
    {
            float v[3];
    }; //等同于使用glm库的 glm::vec3*/

    //表示该面片的第i个顶点的 位置索引/纹理坐标索引/法向量索引
    struct VertexDefine {
      int positionIndex;
      int texcoordIndex;
      int normalIndex;
    };

    //暂存obj文件的三类顶点数据
    std::vector<glm::vec3> positions; //顶点位置信息列表 等同于使用glm库的
                                      //std::vector<glm::vec3>& points,
    std::vector<glm::vec2> texcoords; //顶点纹理坐标列表
    std::vector<glm::vec3> normals;   //顶点法线信息列表

    std::vector<unsigned int> objIndexes; // -> opengl indexes  索引数据
    std::vector<VertexDefine> vertices; // -> opengl vertexes   三角面片顶点
                                        // 位置索引/纹理坐标索引/法向量索引

    std::stringstream ssObjFile(fileContent); //字符流
    char fileLine[256];
    std::string temp; //暂存读取的数据
    //读取数据并存储
    while (!ssObjFile.eof()) {
      memset(fileLine, 0, 256);
      ssObjFile.getline(fileLine, 256); //获取
      if (strlen(fileLine) > 0) {
        std::stringstream ssLine(fileLine);

        //根据开头字符判断接下来的是哪种数据
        if (fileLine[0] == 'v') {
          if (fileLine[1] == 't') {
            // vertex coord 顶点纹理坐标
            ssLine >> temp;

            glm::vec2 vi;
            ssLine >> vi[0];
            ssLine >> vi[1];
            texcoords.push_back(vi);
          } else if (fileLine[1] == 'n') {
            // normal  顶点法线坐标
            ssLine >> temp;
            glm::vec3 vi;
            ssLine >> vi[0];
            ssLine >> vi[1];
            ssLine >> vi[2];
            normals.push_back(vi);
          } else {
            // position  顶点位置坐标
            ssLine >> temp;
            glm::vec3 vi;
            ssLine >> vi[0];
            ssLine >> vi[1];
            ssLine >> vi[2];
            positions.push_back(vi);
          }
        } else if (fileLine[0] == 'f') {
          // face  三角面片
          ssLine >> temp; // 'f'
          std::string vertexStr;
          for (int i = 0; i < 3; i++) {
            ssLine >> vertexStr;
            size_t pos = vertexStr.find_first_of('/'); //根据/分割字符串
            std::string positionIndexStr =
                vertexStr.substr(0, pos); //获取位置索引
            size_t pos2 = vertexStr.find_first_of('/', pos + 1);
            std::string texcoordIndexStr =
                vertexStr.substr(pos + 1, pos2 - pos - 1); //获取纹理索引
            std::string normalIndexStr = vertexStr.substr(
                pos2 + 1, vertexStr.length() - pos2 - 1); //获取法线信息
            VertexDefine vd; //声明一个三角面片顶点
            vd.positionIndex =
                atoi(positionIndexStr.c_str()) -
                1; // atio:把字符串转换为整型数,其要求参数是字符数组
                   // .c_str()是string类的函数,把string转换为字符数组
            vd.texcoordIndex = atoi(texcoordIndexStr.c_str()) - 1;
            vd.normalIndex = atoi(normalIndexStr.c_str()) - 1;

            int nCurrentIndex = -1; // indexes
            //检查是否存在
            size_t nCurrentVerticeCount = vertices.size();
            for (size_t j = 0; j < nCurrentVerticeCount; j++) {
              //数据已存在vertices中,设置当前索引值
              if (vertices[j].positionIndex == vd.positionIndex &&
                  vertices[j].texcoordIndex == vd.texcoordIndex &&
                  vertices[j].normalIndex == vd.normalIndex) {
                nCurrentIndex = j;
                break;
              }
            }
            //不存在,创建新顶点数据,插入vertices数组
            if (nCurrentIndex == -1) {
              // create new vertice
              nCurrentIndex = vertices.size();
              vertices.push_back(vd); //插入三角面片顶点数据
            }
            objIndexes.push_back(
                nCurrentIndex); //插入当前索引  f的一个数据就是一条索引
          }
        }
      }
    }
    //读取完毕

    //存储索引信息,为生成索引缓冲对象做准备
    // objIndexes->indexes buffer -> ibo
    indexCount = (int)objIndexes.size();
    *indexes = new unsigned int[indexCount];
    for (int i = 0; i < indexCount; i++) {
      (*indexes)[i] = objIndexes[i];
    }
    //存储顶点数据,为生成顶点缓冲对象做准备
    // vertices -> vertexes -> vbo
    vertexCount = (int)vertices.size();
    VertexData *vertexes = new VertexData[vertexCount]; //定义一个结构体数组指针
    //将之前暂存的值复制给结构体数组
    for (int i = 0; i < vertexCount; ++i) {
      vertexes[i].position = positions[vertices[i].positionIndex];
      vertexes[i].texcoord = texcoords[vertices[i].texcoordIndex];
      vertexes[i].normal = normals[vertices[i].normalIndex];
    }
    return vertexes; //返回
  }
  return nullptr;
}