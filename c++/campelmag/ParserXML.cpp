#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>

#include "ParserXML.hpp"

//Referencias
//http://www.boost.org/doc/libs/1_42_0/doc/html/boost_propertytree/tutorial.html
//http://www.technical-recipes.com/2014/using-boostproperty_tree/

void conf::load(const std::string &filename)
{

    // Create empty property tree object
    using boost::property_tree::ptree;
    ptree pt;

    read_xml(filename, pt);

    // Leitura dos dados do vao
    nome = pt.get<std::string>("projeto.nome");
		grafico = pt.get<int>("projeto.grafico");
		tensao = pt.get<double>("projeto.caso.tensao");
		potencia = pt.get<double>("projeto.caso.pot");

		angabc[0] = pt.get<double>("projeto.caso.angabc.a");
		angabc[1] = pt.get<double>("projeto.caso.angabc.b");
		angabc[2] = pt.get<double>("projeto.caso.angabc.c");

		linhamed[0] = pt.get<double>("projeto.caso.linhamed.xmin");
		linhamed[1] = pt.get<double>("projeto.caso.linhamed.xmax");
		linhamed[2] = pt.get<double>("projeto.caso.linhamed.passo");
		linhamed[3] = pt.get<double>("projeto.caso.linhamed.hmed");

    coefm = pt.get<double>("projeto.caso.coefm");
    hmax = pt.get<double>("projeto.caso.hmax");
    flecha = pt.get<double>("projeto.caso.flecha");
    espacsubcond = pt.get<double>("projeto.caso.espacsubcond");
    diamcabo = pt.get<double>("projeto.caso.diamcabo");
    ncond = pt.get<double>("projeto.caso.ncond");

		hmin = {hmax-flecha-(espacsubcond/2.0)};
		corrente = potencia/tensao;

		pxfeixes[0] = pt.get<double>("projeto.caso.pxfeixes.e");
		pxfeixes[1] = pt.get<double>("projeto.caso.pxfeixes.c");
		pxfeixes[2] = pt.get<double>("projeto.caso.pxfeixes.d");
}

void conf::save(const std::string &filename)
{

    // Create empty property tree object
    using boost::property_tree::ptree;
    ptree pt;

    // Put log filename in property tree
    pt.put("projeto.nome", nome);

    // Put debug level in property tree
    pt.put("projeto.vao.tensao", tensao);
    pt.put("projeto.cabo.pot", potencia);

    // Write property tree to XML file
    write_xml(filename, pt, std::locale(),
        boost::property_tree::xml_writer_make_settings<ptree::key_type>(' ', 1u));

}


void relat::save(const std::string &filename)
{

    // Create empty property tree object
    using boost::property_tree::ptree;
    ptree pt;

    // Put log filename in property tree
    pt.put("projeto.titulo", titulo);
    pt.put("projeto.relatorio.tensao", tensao);
    pt.put("projeto.relatorio.potencia", potencia);
		pt.put("projeto.relatorio.corrente", corrente);
		pt.put("projeto.relatorio.hmin", hmin);
		pt.put("projeto.relatorio.hmax", hmax);
		pt.put("projeto.relatorio.hmax", hmax);
		pt.put("projeto.relatorio.distfeixe", distfeixe);
		pt.put("projeto.relatorio.npmed", npmed);
		pt.put("projeto.relatorio.campel", campel);
		pt.put("projeto.relatorio.campmag", campmag);

    // Write property tree to XML file
    write_xml(filename, pt, std::locale(),
        boost::property_tree::xml_writer_make_settings<ptree::key_type>(' ', 1u));

}
