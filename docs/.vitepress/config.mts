import { defineConfig } from "vitepress";
import llmstxt from "vitepress-plugin-llms";

export const SITE_URL = "https://muhammad-fiaz.github.io/json-c/";
export const SITE_NAME = "JSON-C";
export const SITE_DESCRIPTION = "JSON-C is a lightweight, modular, dependency-free JSON library for C and C++ with parsing, serialization, DOM editing, mutation, and diagnostics.";
export const GA_ID = "G-6BVYCRK57P";
export const GTM_ID = "GTM-P4M9T8ZR";
export const ADSENSE_CLIENT_ID = "ca-pub-2040560600290490";
export const KEYWORDS = "json, json parser, json serializer, c, c++, c library, dom, validation, utf-8, unicode, embedded, portable, performance, ownership";

const authorSchema = {
  "@type": "Person",
  name: "Muhammad Fiaz",
  url: "https://muhammadfiaz.com",
  sameAs: [
    "https://github.com/muhammad-fiaz",
    "https://www.linkedin.com/in/muhammad-fiaz-",
    "https://x.com/muhammadfiaz_",
  ],
};

export default defineConfig({
  lang: "en-US",
  title: SITE_NAME,
  description: SITE_DESCRIPTION,
  base: "/json-c/",
  lastUpdated: true,
  cleanUrls: false,
  sitemap: {
    hostname: SITE_URL,
  },
  vite: {
    plugins: [llmstxt()],
  },
  head: [
    ["meta", { name: "title", content: SITE_NAME }],
    ["meta", { name: "description", content: SITE_DESCRIPTION }],
    ["meta", { name: "keywords", content: KEYWORDS }],
    ["meta", { name: "author", content: "Muhammad Fiaz" }],
    ["meta", { name: "robots", content: "index, follow" }],
    ["meta", { name: "language", content: "English" }],
    ["meta", { name: "revisit-after", content: "7 days" }],
    ["meta", { name: "generator", content: "VitePress" }],
    ["meta", { name: "viewport", content: "width=device-width, initial-scale=1.0" }],
    ["meta", { property: "og:type", content: "website" }],
    ["meta", { property: "og:url", content: SITE_URL }],
    ["meta", { property: "og:title", content: SITE_NAME }],
    ["meta", { property: "og:description", content: SITE_DESCRIPTION }],
    ["meta", { property: "og:image", content: `${SITE_URL}cover.svg` }],
    ["meta", { property: "og:image:width", content: "1200" }],
    ["meta", { property: "og:image:height", content: "630" }],
    ["meta", { property: "og:image:alt", content: "json-c - Lightweight ISO C JSON library" }],
    ["meta", { property: "og:site_name", content: SITE_NAME }],
    ["meta", { property: "og:locale", content: "en_US" }],
    ["meta", { name: "twitter:card", content: "summary_large_image" }],
    ["meta", { name: "twitter:url", content: SITE_URL }],
    ["meta", { name: "twitter:title", content: SITE_NAME }],
    ["meta", { name: "twitter:description", content: SITE_DESCRIPTION }],
    ["meta", { name: "twitter:image", content: `${SITE_URL}cover.svg` }],
    ["meta", { name: "twitter:image:alt", content: "json-c - Lightweight ISO C JSON library" }],
    ["meta", { name: "twitter:site", content: "@muhammadfiaz_" }],
    ["meta", { name: "twitter:creator", content: "@muhammadfiaz_" }],
    ["link", { rel: "canonical", href: SITE_URL }],
    ["link", { rel: "icon", href: "/json-c/logo.svg" }],
    ["link", { rel: "apple-touch-icon", href: "/json-c/logo.svg" }],
    ["meta", { name: "theme-color", content: "#0f172a" }],
    ["meta", { name: "msapplication-TileColor", content: "#0f172a" }],
    ["script", { async: "", src: `https://www.googletagmanager.com/gtag/js?id=${GA_ID}` }],
    [
      "script",
      {},
      `window.dataLayer = window.dataLayer || [];
function gtag(){dataLayer.push(arguments);}
gtag('js', new Date());
gtag('config', '${GA_ID}');`,
    ],
    ...(GTM_ID
      ? ([
          [
            "script",
            {},
            `(function(w,d,s,l,i){w[l]=w[l]||[];w[l].push({'gtm.start': new Date().getTime(),event:'gtm.js'});var f=d.getElementsByTagName(s)[0],j=d.createElement(s),dl=l!='dataLayer'?'&l='+l:'';j.async=true;j.src='https://www.googletagmanager.com/gtm.js?id='+i+dl;f.parentNode.insertBefore(j,f);})(window,document,'script','dataLayer','${GTM_ID}');`,
          ],
          [
            "noscript",
            {},
            `<iframe src="https://www.googletagmanager.com/ns.html?id=${GTM_ID}" height="0" width="0" style="display:none;visibility:hidden"></iframe>`,
          ],
        ] as [string, Record<string, string>, string][])
      : []),
    [
      "script",
      {
        async: "",
        src: `https://pagead2.googlesyndication.com/pagead/js/adsbygoogle.js?client=${ADSENSE_CLIENT_ID}`,
        crossorigin: "anonymous",
      },
    ],
  ],
  ignoreDeadLinks: [/.*\.c$/, /.*\.h$/, /.*\.svg$/],
  transformPageData(pageData: any) {
    const pageTitle = pageData.title || SITE_NAME;
    const pageDescription = pageData.description || SITE_DESCRIPTION;
    const normalizedPath = pageData.relativePath.replace(/\.md$/, "").replace(/(^|\/)index$/, "$1").replace(/\/$/, "");
    const canonicalUrl = normalizedPath.length > 0 ? `${SITE_URL}${normalizedPath}` : SITE_URL;
    const isHome = pageData.relativePath === "index.md";
    const lastUpdated = pageData.lastUpdated ? new Date(pageData.lastUpdated).toISOString() : new Date().toISOString();

    pageData.frontmatter.head ??= [];
    pageData.frontmatter.head.push(
      ["link", { rel: "canonical", href: canonicalUrl }],
      ["meta", { property: "og:title", content: pageTitle }],
      ["meta", { property: "og:url", content: canonicalUrl }],
      ["meta", { name: "twitter:title", content: pageTitle }],
      ["meta", { name: "twitter:url", content: canonicalUrl }]
    );

    if (pageData.frontmatter.description) {
      pageData.frontmatter.head.push(
        ["meta", { property: "og:description", content: pageData.frontmatter.description }],
        ["meta", { name: "description", content: pageData.frontmatter.description }],
        ["meta", { name: "twitter:description", content: pageData.frontmatter.description }]
      );
    }

    const graph: any[] = [];

    if (isHome) {
      graph.push({
        "@type": "WebSite",
        name: SITE_NAME,
        url: SITE_URL,
        description: SITE_DESCRIPTION,
        author: authorSchema,
      });
    }

    const primarySchema: Record<string, any> = {
      "@type": isHome ? "SoftwareApplication" : "TechArticle",
      name: isHome ? SITE_NAME : pageTitle,
      description: pageDescription,
      url: canonicalUrl,
      image: `${SITE_URL}cover.svg`,
      author: authorSchema,
      publisher: {
        "@type": "Organization",
        name: SITE_NAME,
        url: SITE_URL,
        logo: {
          "@type": "ImageObject",
          url: `${SITE_URL}logo.svg`,
        },
      },
    };

    if (isHome) {
      Object.assign(primarySchema, {
        applicationCategory: "DeveloperApplication",
        operatingSystem: "Cross-platform",
        programmingLanguage: "C",
        offers: {
          "@type": "Offer",
          price: "0",
          priceCurrency: "USD",
        },
        downloadUrl: "https://github.com/muhammad-fiaz/json-c",
        softwareVersion: "0.1.0",
        license: "https://opensource.org/licenses/MIT",
      });
    } else {
      const section = pageData.relativePath.split("/")[0]?.charAt(0).toUpperCase() + pageData.relativePath.split("/")[0]?.slice(1) || "Documentation";
      Object.assign(primarySchema, {
        headline: pageTitle,
        articleSection: section,
        mainEntityOfPage: {
          "@type": "WebPage",
          "@id": canonicalUrl,
        },
        datePublished: "2026-01-01T00:00:00Z",
        dateModified: lastUpdated,
      });
    }
    graph.push(primarySchema);

    const breadcrumbs: any[] = [
      {
        "@type": "ListItem",
        position: 1,
        name: "Home",
        item: SITE_URL,
      },
    ];

    if (!isHome) {
      const pathParts = pageData.relativePath.replace(/\.md$/, "").split("/").filter(Boolean);
      const breadcrumbParts = pathParts[pathParts.length - 1] === "index" ? pathParts.slice(0, -1) : pathParts;

      if (breadcrumbParts.length === 1) {
        breadcrumbs.push({
          "@type": "ListItem",
          position: 2,
          name: pageTitle,
          item: canonicalUrl,
        });
      } else if (breadcrumbParts.length > 1) {
        let currentPath = SITE_URL;

        breadcrumbParts.slice(0, -1).forEach((part: string, index: number) => {
          currentPath += `${part}/`;
          const name = part.split("-").map((segment: string) => segment.charAt(0).toUpperCase() + segment.slice(1)).join(" ");
          breadcrumbs.push({
            "@type": "ListItem",
            position: index + 2,
            name,
            item: currentPath,
          });
        });

        breadcrumbs.push({
          "@type": "ListItem",
          position: breadcrumbParts.length + 1,
          name: pageTitle,
          item: canonicalUrl,
        });
      }
    }

    graph.push({
      "@type": "BreadcrumbList",
      itemListElement: breadcrumbs,
    });

    pageData.frontmatter.head.push([
      "script",
      { type: "application/ld+json" },
      JSON.stringify({
        "@context": "https://schema.org",
        "@graph": graph,
      }),
    ]);
  },
  themeConfig: {
    logo: "/json-c/logo.svg",
    siteTitle: "json-c",
    nav: [
      { text: "Home", link: "/" },
      { text: "Guide", link: "/guide/getting-started" },
      { text: "API", link: "/api/" },
      { text: "Examples", link: "/examples/" },
      { text: "C and C++", link: "/guide/c-and-cpp" },
      { text: "FAQ", link: "/guide/faq" },
      { text: "GitHub", link: "https://github.com/muhammad-fiaz/json-c" },
    ],
    sidebar: [
      {
        text: "Getting Started",
        items: [
          { text: "Introduction", link: "/guide/introduction" },
          { text: "Getting Started", link: "/guide/getting-started" },
          { text: "Configuration", link: "/guide/configuration" },
          { text: "C and C++", link: "/guide/c-and-cpp" },
          { text: "File Workflows", link: "/guide/file-workflows" },
          { text: "Operations", link: "/guide/operations" },
          { text: "Installation", link: "/guide/installation" },
          { text: "Architecture", link: "/guide/architecture" },
          { text: "Internals", link: "/guide/internals" },
        ],
      },
      {
        text: "Project",
        items: [
          { text: "Contributing", link: "/guide/contributing" },
          { text: "FAQ", link: "/guide/faq" },
          { text: "Code of Conduct", link: "/guide/code-of-conduct" },
        ],
      },
      {
        text: "Guides",
        items: [
          { text: "Validation", link: "/guide/validation" },
          { text: "Performance", link: "/guide/efficiency" },
        ],
      },
      {
        text: "API Reference",
        items: [
          { text: "Overview", link: "/api/" },
          { text: "Types", link: "/api/types" },
          { text: "Parser", link: "/api/parser" },
          { text: "Serializer", link: "/api/export" },
          { text: "Errors", link: "/api/errors" },
          { text: "Constants", link: "/api/constants" },
        ],
      },
      {
        text: "Examples",
        items: [{ text: "Examples", link: "/examples/" }],
      },
    ],
    socialLinks: [
      { icon: "github", link: "https://github.com/muhammad-fiaz/json-c" },
      { icon: "twitter", link: "https://x.com/muhammadfiaz_" },
    ],
    footer: {
      message: "MIT licensed and dependency-free.",
      copyright: "Copyright © 2026 Muhammad Fiaz",
    },
    search: { provider: "local" },
    editLink: {
      pattern: "https://github.com/muhammad-fiaz/json-c/edit/main/docs/:path",
      text: "Edit this page on GitHub",
    },
    lastUpdated: {
      text: "Last updated",
      formatOptions: {
        dateStyle: "medium",
        timeStyle: "short",
      },
    },
  },
});