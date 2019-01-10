#include "stdafx.h"
#include "Config.h"
#include "tinyxml/tinyxml.h"
#include <string>
#include <vector>
using namespace std;

/** Returns a std::stringVector that contains all the substd::strings delimited
   by the characters in the passed <code>delims</code> argument.
   @param 
   delims A list of delimiter characters to split by
   @param 
   maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
   parameters is > 0, the splitting process will stop after this many splits, left to right.
   */
std::vector<string> split( const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0)
{
	// 死循环监测，必需放在第一行
	//CEndlessLoopGuard stEndlessLoopGuard(CEndlessLoop::get_singleton_ptr(), __FUNCTION__);

	std::vector<string> ret;
	unsigned int numSplits = 0;

	// Use STL methods 
	size_t start, pos;
	start = 0;
	do 
	{
		pos = str.find_first_of(delims, start);
		if (pos == start)
		{
			// Do nothing
			start = pos + 1;
		}
		else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
		{
			// Copy the rest of the std::string
			ret.push_back( str.substr(start) );
			break;
		}
		else
		{
			// Copy up to delimiter
			ret.push_back( str.substr(start, pos - start) );
			start = pos + 1;
		}
		// parse up to next real data
		start = str.find_first_not_of(delims, start);
		++numSplits;

	} while (pos != std::string::npos);
	return ret;
}

BRNNConfig::BRNNConfig(const char *xmlFileName/*=0*/)
{
	memset(this,0,sizeof(*this));
	if(xmlFileName!=NULL)
		LoadData(xmlFileName);
}

BRNNConfig & BRNNConfig::Instance()
{
	static BRNNConfig g_BRNNConfig("BRNNConfig.xml");//配置缓冲，只读取了xml文件一次
	return g_BRNNConfig;
}

//返回值
//	0			成功
//	1			没有加载文件或打开文件失败
int BRNNConfig::LoadData(const char *xmlFileName)
{
	if(xmlFileName==NULL)
		return 1;
	TiXmlDocument* m_xmlDoc = new TiXmlDocument(xmlFileName);
	if (!m_xmlDoc->LoadFile())
	{
		delete m_xmlDoc;
		return 1;
	}
	else
	{
#define SetINT(Node,Attr) {const char*##Attr##=##Node##->Attribute(#Attr);if(##Attr##!=NULL)this->m_##Attr##=atoi(##Attr##);}
#define SetDOUBLE(Node,Attr) {const char*##Attr##=##Node##->Attribute(#Attr);if(##Attr##!=NULL)this->m_##Attr##=atof(##Attr##);}
		TiXmlElement *root = m_xmlDoc->FirstChildElement("config");
		if (root)
		{
			TiXmlElement* Node1=root->FirstChildElement("FixCard");
			if(Node1)
			{
				SetINT(Node1,Enable)
				SetINT(Node1,Refresh)
                for(int i=0;i<5;i++)
				{
					char szKey[40]={0};
					sprintf(szKey,"Card%d",i+1);
					vector<string> vecCard;
					const char*Card=Node1->Attribute(szKey);
					if(Card!=NULL)
					{
						vecCard=split(Card,",");
					}
					if(vecCard.size()==5)
					{
						for(int j=0;j<5;j++)
						{
							char* pp=NULL;
							m_cbCardData[i][j]=strtol(vecCard[j].c_str(),&pp,16);
						}
					}
					else
					{
						m_Enable=0;
					}
				}
			}
		}
#undef SetINT
#undef SetDOUBLE
	}
	delete m_xmlDoc;
	this->m_HasLoadOK=true;
	return 0;	
}



