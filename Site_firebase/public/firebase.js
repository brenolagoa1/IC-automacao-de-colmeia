import { initializeApp } from "https://www.gstatic.com/firebasejs/9.8.4/firebase-app.js";
import { getAnalytics } from "https://www.gstatic.com/firebasejs/9.8.4/firebase-analytics.js";

// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyAIBdqdxkjcbsdibidcbidasvk68E",
  authDomain: "nomeaaplicacao.firebaseapp.com",
  databaseURL: "https://nomeapp-default-rtdb.firebaseio.com",
  projectId: "id projeto",
  storageBucket: "id.appspot.com",
  messagingSenderId: "291740444080",
  appId: "1:291740837080:web:2725ddfadda2ed541c7d63ad",
  measurementId: "G-H7XQMQ1283"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);
